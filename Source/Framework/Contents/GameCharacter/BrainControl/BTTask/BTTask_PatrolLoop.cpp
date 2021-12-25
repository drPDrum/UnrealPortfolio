#include "BTTask_PatrolLoop.h"

#include "Framework/Contents/GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"

EBTNodeResult::Type UBTTask_PatrolLoop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (result == EBTNodeResult::Type::InProgress)
	{
		auto pBrainCtrl = Cast<ABrainController>(OwnerComp.GetAIOwner());
		auto pOwner = pBrainCtrl->GetOwner();

		SetPatrolPoint(pBrainCtrl, pOwner, true);
	}

	return result;
}

void UBTTask_PatrolLoop::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto pBrainCtrl = Cast<ABrainController>(OwnerComp.GetAIOwner());
	auto pOwner = pBrainCtrl->GetOwner();

	SetPatrolPoint(pBrainCtrl, pOwner, false);
	
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTTask_PatrolLoop::SetPatrolPoint(ABrainController* pBrainCtrl, AGameCharacter* pOwner, const bool& bForced)
{
	auto& pSpawnInfo = pBrainCtrl->GetSpawnInfo();
	if (pSpawnInfo)
	{
		// 패트롤 포인트에 가까워 졌다면
		if (bForced || pSpawnInfo->IsClosePatrolPoint(pBrainCtrl->GetPawn()->GetActorLocation(), 100.f))
		{
			// EndPoint에 도달했다면 리셋시킨다.
			if (pSpawnInfo->IsPatrolEndPoint())
				pSpawnInfo->ResetPatrolPoint();
			else
				pSpawnInfo->NextPatrolPoint(pOwner->GetMovementComponent()->GetMaxSpeed());

			// 목표 포지션 세팅
			pBrainCtrl->SetLookInfo(FRotationMatrix::MakeFromZX(pOwner->GetActorUpVector(), pSpawnInfo->GetPatrolLook().GetForwardVector()).ToQuat());
			pBrainCtrl->SetTarget(pSpawnInfo->GetPatrolPoint());
			pBrainCtrl->OnInput_MoveAxisX(0.f);
			pBrainCtrl->OnInput_MoveAxisY(1.f);
		}
	}
}