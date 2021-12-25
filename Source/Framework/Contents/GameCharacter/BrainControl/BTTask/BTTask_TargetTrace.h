// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Framework/Contents/GameCharacter/BrainControl/SurveyInfo.h"
#include "BTTask_TargetTrace.generated.h"

UENUM()
enum class ETraceCheckResult : uint8
{
	Exception,
	OnReach,
	NotReach,
	BetweenWall,
};

class AGameCharacter;

UCLASS()
class FRAMEWORK_API UBTTask_TargetTrace : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="TraceTarget"))
	ESurveyTargetTypes  m_TraceTargetType;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="TargetDistance"))
	float  m_TraceTargetDistance;

	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="ActionNameForTrace"))
	FString m_ActionNameForTrace;

	/** cached description */
	UPROPERTY()
	FString CachedDescription;
public:
	UBTTask_TargetTrace();
	virtual ~UBTTask_TargetTrace() override;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	ETraceCheckResult CheckReach(AGameCharacter* pOwner, const SurveyGameCharacterInfo& targetInfo);

protected:
	
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR

	/** describe decorator and cache it */
	virtual void BuildDescription();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
#endif
};
