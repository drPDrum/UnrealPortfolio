// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Survey.generated.h"

/**
 * 
 */
UCLASS()
class FRAMEWORK_API UBTTask_Survey : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_Survey();
	virtual ~UBTTask_Survey() override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
