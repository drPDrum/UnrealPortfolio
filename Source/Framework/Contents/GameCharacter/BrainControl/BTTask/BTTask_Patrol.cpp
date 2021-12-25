// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DataLoadLib/DataFile/DataStructure/CCharacterData.h"
#include "Framework/Contents/GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionController.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	bNotifyTick = true;
}

UBTTask_Patrol::~UBTTask_Patrol()
{
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// owner 체크
	auto pOwner = Cast<AGameCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pOwner)
		return EBTNodeResult::Succeeded;

	if (CheckEnemy(pOwner))
		return EBTNodeResult::Succeeded;

	// 패트롤 액션 실행
	if (!m_ActionName.IsEmpty())
	{
		auto& pActionCtrl = pOwner->GetActionCtrl();
		if (!pActionCtrl->IsPlaying(m_ActionName))
			pActionCtrl->PlayAction(m_ActionName);
	}
	else
	{
		LOG_ERROR(TEXT("Patrol ActionName is Empty"));
	}

	// 체크 인터벌 초기화
	auto pBB = OwnerComp.GetBlackboardComponent();
	pBB->SetValueAsFloat(KEY_BB_TICK_REMAIN_TIME, 0.5f);
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	auto pOwner = Cast<AGameCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pOwner)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	// 체크 인터벌 업데이트
	auto pBB = OwnerComp.GetBlackboardComponent();
	float fCheckRemainTime = pBB->GetValueAsFloat(KEY_BB_TICK_REMAIN_TIME);
	fCheckRemainTime -= DeltaSeconds;

	// 체크 인터벌 확인
	if(fCheckRemainTime <= 0.f)
	{
		// 체크
		if (CheckEnemy(pOwner))
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		// 체크 인터벌 초기화
		pBB->SetValueAsFloat(KEY_BB_TICK_REMAIN_TIME, 0.5f);
	}
	else
	{
		pBB->SetValueAsFloat(KEY_BB_TICK_REMAIN_TIME, fCheckRemainTime);
	}
}

bool UBTTask_Patrol::CheckEnemy(AGameCharacter*& pOwner)
{
	// 찾을 오브젝트 Flag
	int32 eccFlag = pOwner->GetTeamInfo()->GetEccFlagEnemy();
	
	// 나를 제외하고 찾는다
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(pOwner);
	
	const auto bHit = pOwner->GetWorld()->OverlapAnyTestByObjectType(
		pOwner->GetActorLocation(),
		pOwner->GetActorRotation().Quaternion(),
		eccFlag,
		FCollisionShape::MakeSphere(pOwner->GetData()->Sight),
		queryParams
		);

	if (bHit)
	{
		// 방향 입력 리셋
		pOwner->GetBrainCtrl()->OnInput_MoveAxisX(0.f);
		pOwner->GetBrainCtrl()->OnInput_MoveAxisY(0.f);

		// Idle 재생
		auto actionName_Idle = TEXT("ActionData_Idle");
		auto& pActionCtrl = pOwner->GetActionCtrl();
		if (!pActionCtrl->IsPlaying(actionName_Idle))
			pActionCtrl->PlayAction(actionName_Idle);
			
		// Battle로 변경
		pOwner->GetBrainCtrl()->SetCurrAIBehavior(EAIBehaviorTypes::Battle);
	}

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(
		pOwner->GetWorld(),
		pOwner->GetActorLocation(),
		pOwner->GetData()->Sight,
		20,
		bHit ? FColor::Purple : FColor::Green,
		false,
		0.3f);
#endif

	return bHit;
}

EBTNodeResult::Type UBTTask_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::AbortTask(OwnerComp, NodeMemory);

	return result;
}

FString UBTTask_Patrol::GetStaticDescription() const
{
	return CachedDescription;
}

#if WITH_EDITOR


void UBTTask_Patrol::BuildDescription()
{
	CachedDescription.Empty();
	CachedDescription.Append(FString::Printf(TEXT("ActionName : %s"), *m_ActionName));
}

void UBTTask_Patrol::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	BuildDescription();
}

void UBTTask_Patrol::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	BuildDescription();
}
#endif