// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckSurvey.generated.h"

UENUM()
enum class EConditionTypes : int16
{
	Friendly_AliveCount,
	Friendly_DieCount,
	Friendly_Highest_HPRate,
	Friendly_Lowest_HPRate,
	Friendly_Nearest_Dist,
	Friendly_Farthest_Dist,
	
	Enemy_AliveCount,
	Enemy_DieCount,
	Enemy_Highest_HPRate,
	Enemy_Lowest_HPRate,
	Enemy_Nearest_Dist,
	Enemy_Farthest_Dist,
};

UENUM()
enum class ECompareTypes : int8
{
	Over,
	OverOrEqual,
	Equal,
	UnderOrEqual,
	Under,
};

USTRUCT()
struct FConditionCheckInfo
{
	GENERATED_BODY()

	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Condition Type"))
	EConditionTypes ConditionType;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Compare Type"))
	ECompareTypes CompareType;
	UPROPERTY(Category=Blackboard, EditAnywhere, meta=(DisplayName="Compare Value"))
	int Value;
};

UCLASS()
class FRAMEWORK_API UBTDecorator_CheckSurvey : public UBTDecorator
{
	UPROPERTY(Category="CheckSurvey", EditAnywhere, meta=(DisplayName="Condition Check Infos"))
	TArray<FConditionCheckInfo> m_ConditionCheckInfos;
	
	/** cached description */
	UPROPERTY()
	FString CachedDescription;
	
	GENERATED_BODY()

protected:
	virtual FString GetStaticDescription() const override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

#if WITH_EDITOR

	/** describe decorator and cache it */
	virtual void BuildDescription();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
#endif
};