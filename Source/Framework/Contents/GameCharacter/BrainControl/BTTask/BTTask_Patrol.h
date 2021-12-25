// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Patrol.generated.h"

class AGameCharacter;

UCLASS()
class FRAMEWORK_API UBTTask_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

#define KEY_BB_TICK_REMAIN_TIME TEXT("TickRemainTime")
	
	UPROPERTY(Category = "PatrolAction", EditAnywhere, meta = (DisplayName = "ActionName"))
	FString m_ActionName;

	/** cached description */
	UPROPERTY()
	FString CachedDescription;
public:
	UBTTask_Patrol();
	virtual ~UBTTask_Patrol() override;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	bool CheckEnemy(AGameCharacter*& pOwner);
protected:
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	/** describe decorator and cache it */
	virtual void BuildDescription();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
#endif
};