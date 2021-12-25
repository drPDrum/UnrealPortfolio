// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PlayAction.generated.h"

class AGameCharacter;
UCLASS()
class FRAMEWORK_API UBTTask_PlayAction : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(Category="PlayAction", EditAnywhere, meta = (DisplayName = "Input"))
	FVector2D m_vInput;
	UPROPERTY(Category="PlayAction", EditAnywhere, meta=(DisplayName="ActionName"))
	FString m_ActionName;

	/** cached description */
	UPROPERTY()
	FString CachedDescription;
public:
	UBTTask_PlayAction();
	virtual ~UBTTask_PlayAction() override;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	bool CheckTrigger(AGameCharacter*& pOwner);
protected:
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	/** describe decorator and cache it */
	virtual void BuildDescription();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
#endif
};