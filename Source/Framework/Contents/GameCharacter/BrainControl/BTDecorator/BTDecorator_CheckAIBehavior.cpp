#include "BTDecorator_CheckAIBehavior.h"

#include "AIController.h"
#include "Framework/Contents/GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "RuntimeUtils/GenericEx/Common.h"


FString UBTDecorator_CheckAIBehavior::GetStaticDescription() const
{
	return CachedDescription;
}

bool UBTDecorator_CheckAIBehavior::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto pGameCharacter = Cast<AGameCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pGameCharacter)
		return false;
	
	return pGameCharacter->GetBrainCtrl()->GetCurrAIBehavior() == m_AIBehaviorType;
}

#if WITH_EDITOR

void UBTDecorator_CheckAIBehavior::BuildDescription()
{
	CachedDescription.Empty();
	CachedDescription.Append(*UEnumToString(m_AIBehaviorType));
}

void UBTDecorator_CheckAIBehavior::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// 변경 사항 시
	Super::PostEditChangeProperty(PropertyChangedEvent);
	BuildDescription();
}

void UBTDecorator_CheckAIBehavior::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	BuildDescription();
}

#endif