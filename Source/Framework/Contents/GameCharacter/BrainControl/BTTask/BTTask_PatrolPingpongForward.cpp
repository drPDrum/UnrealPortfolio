#include "BTTask_PatrolPingpongForward.h"

#include "Framework/Contents/GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"

EBTNodeResult::Type UBTTask_PatrolPingpongForward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (result == EBTNodeResult::Type::InProgress)
	{
		auto pBrainCtrl = Cast<ABrainController>(OwnerComp.GetAIOwner());
		auto pOwner = pBrainCtrl->GetOwner();

		// End 도달시 true
		if (SetPatrolPoint(pBrainCtrl, pOwner, true))
			return EBTNodeResult::Failed;
	}

	return result;
}

void UBTTask_PatrolPingpongForward::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto pBrainCtrl = Cast<ABrainController>(OwnerComp.GetAIOwner());
	auto pOwner = pBrainCtrl->GetOwner();

	if (SetPatrolPoint(pBrainCtrl, pOwner, false))
		return FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}


bool UBTTask_PatrolPingpongForward::SetPatrolPoint(ABrainController* pBrainCtrl, AGameCharacter* pOwner, const bool& bForced)
{
	auto& pSpawnInfo = pBrainCtrl->GetSpawnInfo();
	if (!pSpawnInfo)
		return false;

	// 패트롤 포인트에 가까워 졌다면
	if (bForced || pSpawnInfo->IsClosePatrolPoint(pOwner->GetActorLocation(), 100))
	{
		// EndPoint에 도달
		if (pSpawnInfo->IsPatrolEndPoint())
			return true;
		
		pSpawnInfo->NextPatrolPoint(pOwner->GetMovementComponent()->GetMaxSpeed());

		// 목표 포지션 세팅
		pBrainCtrl->SetLookInfo(FRotationMatrix::MakeFromZX(pOwner->GetActorUpVector(), pSpawnInfo->GetPatrolLook().GetForwardVector()).ToQuat());
		pBrainCtrl->SetTarget(pSpawnInfo->GetPatrolPoint());
		pBrainCtrl->OnInput_MoveAxisX(0.f);
		pBrainCtrl->OnInput_MoveAxisY(1.f);
	}

	return false;
}
