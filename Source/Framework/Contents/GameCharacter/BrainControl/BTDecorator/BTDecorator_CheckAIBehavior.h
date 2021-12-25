#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "../../GameCharacter_Define.h"
#include "BTDecorator_CheckAIBehavior.generated.h"

UCLASS()
class FRAMEWORK_API UBTDecorator_CheckAIBehavior : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(Category="Check AI Behavior", EditAnywhere, meta=(DisplayName="AI BehaviorType"))
	EAIBehaviorTypes m_AIBehaviorType;
	
	/** cached description */
	UPROPERTY()
	FString CachedDescription;
	
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