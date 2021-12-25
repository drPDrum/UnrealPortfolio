// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TargetTrace.h"

// #include "Blueprint/AIBlueprintHelperLibrary.h"
#include "DataLoadLib/DataFile/DataStructure/CCharacterData.h"
#include "Framework/Contents/GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionController.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "Framework/Contents/GameCharacter/BrainControl/SurveyInfo.h"
#include "RuntimeUtils/GenericEx/Common.h"

UBTTask_TargetTrace::UBTTask_TargetTrace()
{
	bNotifyTick = true;
}

UBTTask_TargetTrace::~UBTTask_TargetTrace()
{
}

// 진입
EBTNodeResult::Type UBTTask_TargetTrace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// owner 체크
	auto pOwner = Cast<AGameCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pOwner)
		return EBTNodeResult::Succeeded;

	// 타겟 체크
	auto& targetInfo = pOwner->GetBrainCtrl()->GetSurveyInfo()->GetSurveyTargetInfo(m_TraceTargetType);
	if (!targetInfo.pBody)
		return EBTNodeResult::Succeeded;

	// Action용 타겟으로 지정
	pOwner->GetBrainCtrl()->SetTarget(targetInfo.pTarget, targetInfo.pBody);

	// 거리 측정 및 Input Update
	switch (CheckReach(pOwner, targetInfo))
	{
	case ETraceCheckResult::Exception:
		return EBTNodeResult::Succeeded;
	case ETraceCheckResult::BetweenWall:
	case ETraceCheckResult::NotReach:
		pOwner->GetActionCtrl()->PlayAction(m_ActionNameForTrace);
		return EBTNodeResult::InProgress;
	case ETraceCheckResult::OnReach:
		return EBTNodeResult::Failed;
	}

	return result;
}

// 진행
void UBTTask_TargetTrace::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// owner 체크
	auto pOwner = Cast<AGameCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pOwner)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	const auto& pBrainCtrl = pOwner->GetBrainCtrl();

	// 콤보 진행을 명시
	pBrainCtrl->SetAIInCombo(true);

	// 타겟 체크
	auto& TargetInfo = pBrainCtrl->GetSurveyInfo()->GetSurveyTargetInfo(m_TraceTargetType);
	if (!TargetInfo.pTarget)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	
	switch (CheckReach(pOwner, TargetInfo))
	{
	case ETraceCheckResult::Exception:
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	case ETraceCheckResult::BetweenWall:
	case ETraceCheckResult::NotReach:
		return;
	case ETraceCheckResult::OnReach:
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
}
	

// 나가기
EBTNodeResult::Type UBTTask_TargetTrace::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

ETraceCheckResult UBTTask_TargetTrace::CheckReach(AGameCharacter* pOwner, const SurveyGameCharacterInfo& targetInfo)
{
	if (!(pOwner && targetInfo.pTarget))
		return ETraceCheckResult::Exception;

	auto vOwnerPos = pOwner->GetActorLocation();
	auto vTargetPos = targetInfo.pBody->GetUnrealWorldTransform().GetLocation();
	auto vToTarget = FVector::VectorPlaneProject(vTargetPos - vOwnerPos, pOwner->GetActorUpVector());

	const auto& pBrainCtrl = pOwner->GetBrainCtrl();
	float fOwnerSight = pOwner->GetData()->Sight;

	// 시야에서 벗어나면 추적 종료
	if(vToTarget.SizeSquared() >= FMath::Square(fOwnerSight))
	{
		pBrainCtrl->SetCurrAIBehavior(static_cast<EAIBehaviorTypes>(pBrainCtrl->GetDefaultAIBehavior()));
		return ETraceCheckResult::Exception;
	}

	// 벽체크
	int32 eccFlag = targetInfo.pTarget->GetTeamInfo()->GetEccFlag();
	eccFlag |= ECC_TO_BITFIELD(ECollisionChannel::ECC_WorldStatic);
	eccFlag |= ECC_TO_BITFIELD(ECollisionChannel::ECC_WorldDynamic);

	FCollisionQueryParams params;
	params.AddIgnoredActor(pOwner);

	// Input 업데이트	
	// Todo :: 중력 방향으로 변경
	pBrainCtrl->SetLookInfo(FRotationMatrix::MakeFromZX(pOwner->GetActorUpVector(), vToTarget.GetSafeNormal()).ToQuat());
	pBrainCtrl->OnInput_MoveAxisX(0.f);
	pBrainCtrl->OnInput_MoveAxisY(1.f);
	pBrainCtrl->RefreshInputDir(false);
	
	TArray<FHitResult> hitResults;
	if (pOwner->GetWorld()->LineTraceMultiByObjectType(
		hitResults,
		pOwner->GetActorLocation(),
		vTargetPos,
		eccFlag,
		params
		))
	{
		for (auto& hitResult : hitResults)
		{
			auto pHitActor = hitResult.GetActor();
			
			// 벽이 없다면
			if (pHitActor == targetInfo.pTarget)
				break;

			// 다른 캐릭터면
			if (pHitActor->IsA(AGameCharacter::StaticClass()))
				continue;

			// 여기라면 뭔가 장애물이 있는 것
			return ETraceCheckResult::BetweenWall;
		}
	}

	// 거리 체크
	{
		FHitResult hitResult;
		bool bHit = targetInfo.pBody->Sweep(
			hitResult,
			vOwnerPos,
			vOwnerPos + vToTarget,
			pOwner->GetActorRotation().Quaternion(),
			pOwner->GetCapsuleComponent()->GetCollisionShape(),
			false
			);

		// 같은 선상에 있다
		if (bHit)
		{
			if (hitResult.Distance <= m_TraceTargetDistance)
				return ETraceCheckResult::OnReach;
		}
		// 다른 높이에 있다
		else
		{
			auto fGoalDist = pOwner->GetCapsuleComponent()->GetScaledCapsuleRadius()
				+ targetInfo.pTarget->GetCapsuleComponent()->GetScaledCapsuleRadius()
				+ m_TraceTargetDistance;

			if (vToTarget.SizeSquared() <= FMath::Square(fGoalDist))
				return ETraceCheckResult::OnReach;
		}
	}

	// 닿지 않음
	return ETraceCheckResult::NotReach;
}

FString UBTTask_TargetTrace::GetStaticDescription() const
{
	return CachedDescription;
}

#if WITH_EDITOR
void UBTTask_TargetTrace::BuildDescription()
{
	CachedDescription.Empty();
	CachedDescription.Append(FString::Printf(
		TEXT("\nType : %s\nDist : %f\nAction : %s"),
		*UEnumToString(m_TraceTargetType),
		m_TraceTargetDistance,
		*m_ActionNameForTrace
		));
}

void UBTTask_TargetTrace::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	BuildDescription();
}

void UBTTask_TargetTrace::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	BuildDescription();
}
#endif