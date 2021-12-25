// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BTTask_Patrol.h"
#include "BTTask_PatrolPingpongForward.generated.h"

class ABrainController;

UCLASS()
class FRAMEWORK_API UBTTask_PatrolPingpongForward : public UBTTask_Patrol
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool SetPatrolPoint(ABrainController* pBrainCtrl, AGameCharacter* pOwner, const bool& bForced);
};