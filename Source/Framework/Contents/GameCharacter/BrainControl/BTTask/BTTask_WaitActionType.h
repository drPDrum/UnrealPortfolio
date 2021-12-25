// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../../GameCharacter_Define.h"
#include "BTTask_WaitActionType.generated.h"

/**
 * 
 */
UCLASS()
class FRAMEWORK_API UBTTask_WaitActionType : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	EActionTypes m_WaitActionType;
protected:
	/** cached description */
	UPROPERTY()
	FString CachedDescription;
public:
	UBTTask_WaitActionType();
	virtual ~UBTTask_WaitActionType() override;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	/** describe decorator and cache it */
	virtual void BuildDescription();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
#endif
};