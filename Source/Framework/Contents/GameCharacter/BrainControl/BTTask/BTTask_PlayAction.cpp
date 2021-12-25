#include "BTTask_PlayAction.h"
#include "Framework/Contents/GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionController.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionRuntime.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionTrigger/ActionTriggerRuntime.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "Framework/Contents/GameCharacter/BrainControl/SurveyInfo.h"

UBTTask_PlayAction::UBTTask_PlayAction()
{
	bNotifyTick = true;
	m_vInput = FVector2D(0.f, 1.f);
}

UBTTask_PlayAction::~UBTTask_PlayAction()
{
}

EBTNodeResult::Type UBTTask_PlayAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// owner 체크
	auto pOwner = Cast<AGameCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pOwner)
		return EBTNodeResult::Succeeded;

	const auto& pBrainCtrl = pOwner->GetBrainCtrl();
	const auto& pActionCtrl = pOwner->GetActionCtrl();

	// 콤보 진행을 명시
	pBrainCtrl->SetAIInCombo(true);

	// 타겟을 바라보게 설정
	auto& targetInfo = pBrainCtrl->GetTargetInfo();
	if (targetInfo.IsValidTarget())
	{
		auto vOwnerPos = pOwner->GetActorLocation();
		auto vTargetPos = targetInfo.GetLocation();
		auto vToTarget = FVector::VectorPlaneProject(vTargetPos - vOwnerPos, pOwner->GetActorUpVector());

		// Todo :: 중력 방향으로 변경
		pBrainCtrl->SetLookInfo(FRotationMatrix::MakeFromZX(pOwner->GetActorUpVector(), vToTarget.GetSafeNormal()).ToQuat());
	}

	// 트리거 체크
	if (CheckTrigger(pOwner))
	{
		// 다음 액션 재생
		pBrainCtrl->OnInput_MoveAxisX(m_vInput.X);
		pBrainCtrl->OnInput_MoveAxisY(m_vInput.Y);
		pBrainCtrl->RefreshInputDir(false);
		
		pActionCtrl->PlayAction(m_ActionName);
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_PlayAction::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	const auto& pActionCtrl = pOwner->GetActionCtrl();

	// 타겟을 바라보게 설정
	auto& targetInfo = pBrainCtrl->GetTargetInfo();
	if (targetInfo.IsValidTarget())
	{
		auto vOwnerPos = pOwner->GetActorLocation();
		auto vTargetPos = targetInfo.GetLocation();
		auto vToTarget = FVector::VectorPlaneProject(vTargetPos - vOwnerPos, pOwner->GetActorUpVector());

		// Todo :: 중력 방향으로 변경
		pBrainCtrl->SetLookInfo(FRotationMatrix::MakeFromZX(pOwner->GetActorUpVector(), vToTarget.GetSafeNormal()).ToQuat());
	}

	// 트리거 체크
	if (CheckTrigger(pOwner))
	{
		// 다음 액션 재생
		pBrainCtrl->OnInput_MoveAxisX(m_vInput.X);
		pBrainCtrl->OnInput_MoveAxisY(m_vInput.Y);
		pBrainCtrl->RefreshInputDir(false);
		
		pActionCtrl->PlayAction(m_ActionName);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

bool UBTTask_PlayAction::CheckTrigger(AGameCharacter*& pOwner)
{
	// 현재 재생중인 액션에 현재 액션 트리거가 담겨있는지 체크
	auto pCrrActionRuntime = pOwner->GetActionCtrl()->GetCrrActionRuntime();
	if (!(pCrrActionRuntime && pCrrActionRuntime->HasNextAction(m_ActionName)))
		return false;
	
	auto pTriggerRuntime = pCrrActionRuntime->GetTriggerRuntime(m_ActionName);
	if (!pTriggerRuntime)
		return false;
	
	// 트리거 시간 체크
	// Todo :: Condition Check
	return pTriggerRuntime->CheckTime(pOwner->GetActionCtrl()->GetActionElapsedTime(),pCrrActionRuntime->GetData()->fLength);
}

EBTNodeResult::Type UBTTask_PlayAction::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::AbortTask(OwnerComp, NodeMemory);
	
	return result;
}

FString UBTTask_PlayAction::GetStaticDescription() const
{
	return CachedDescription;
}

#if WITH_EDITOR


void UBTTask_PlayAction::BuildDescription()
{
	CachedDescription.Empty();
	CachedDescription.Append(FString::Printf(TEXT("ActionName : %s"), *m_ActionName));
	CachedDescription.Append(FString::Printf(TEXT("\nInput : V(%.2f) / H(%.2f)"), m_vInput.Y, m_vInput.X));
}

void UBTTask_PlayAction::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	BuildDescription();
}

void UBTTask_PlayAction::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	BuildDescription();
}
#endif