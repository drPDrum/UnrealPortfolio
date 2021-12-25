#include "BTDecorator_CheckSurvey.h"
#include "../../../GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "Framework/Contents/GameCharacter/BrainControl/SurveyInfo.h"
#include "RuntimeUtils/GenericEx/Common.h"

FString UBTDecorator_CheckSurvey::GetStaticDescription() const
{
	return CachedDescription;
}

bool UBTDecorator_CheckSurvey::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto pGameCharacter = Cast<AGameCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pGameCharacter)
		return false;
	
	auto pSurveyInfo = pGameCharacter->GetBrainCtrl()->GetSurveyInfo();
	if (!pSurveyInfo)
		return false;

	for (int i = 0 ; i < m_ConditionCheckInfos.Num(); ++i)
	{
		const auto& conditionCheckInfo = m_ConditionCheckInfos[i];

		// 벨류 세팅
		int crrValue = 0;
		switch (conditionCheckInfo.ConditionType)
		{
			case EConditionTypes::Friendly_AliveCount		: crrValue = pSurveyInfo->nCount_Friendly_Alive;										break;
			case EConditionTypes::Friendly_DieCount			: crrValue = pSurveyInfo->nCount_Friendly_Die;											break;
			case EConditionTypes::Friendly_Highest_HPRate	: crrValue = FMath::RoundToInt(pSurveyInfo->Friendly_HPRateHighest.fValue * 1000.f);	break;
			case EConditionTypes::Friendly_Lowest_HPRate	: crrValue = FMath::RoundToInt(pSurveyInfo->Friendly_HPRateNearest.fValue * 1000.f);	break;
			case EConditionTypes::Friendly_Nearest_Dist		: crrValue = FMath::RoundToInt(pSurveyInfo->Friendly_HPRateHighest.fValue);				break;
			case EConditionTypes::Friendly_Farthest_Dist	: crrValue = FMath::RoundToInt(pSurveyInfo->Friendly_HPRateNearest.fValue);				break;
			case EConditionTypes::Enemy_AliveCount			: crrValue = pSurveyInfo->nCount_Enemy_Alive;											break;
			case EConditionTypes::Enemy_DieCount			: crrValue = pSurveyInfo->nCount_Enemy_Die;												break;
			case EConditionTypes::Enemy_Highest_HPRate		: crrValue = FMath::RoundToInt(pSurveyInfo->Enemy_HPRateHighest.fValue * 1000.f);		break;
			case EConditionTypes::Enemy_Lowest_HPRate		: crrValue = FMath::RoundToInt(pSurveyInfo->Enemy_HPRateNearest.fValue * 1000.f);		break;
			case EConditionTypes::Enemy_Nearest_Dist		: crrValue = FMath::RoundToInt(pSurveyInfo->Enemy_HPRateHighest.fValue);				break;
			case EConditionTypes::Enemy_Farthest_Dist		: crrValue = FMath::RoundToInt(pSurveyInfo->Enemy_HPRateNearest.fValue);				break;
		}

		// 조건 체크
		switch(conditionCheckInfo.CompareType)
		{
			case ECompareTypes::Over:			return conditionCheckInfo.Value <  crrValue;
			case ECompareTypes::OverOrEqual:	return conditionCheckInfo.Value <= crrValue;
			case ECompareTypes::Equal:			return conditionCheckInfo.Value == crrValue;
			case ECompareTypes::UnderOrEqual:	return conditionCheckInfo.Value >= crrValue;
			case ECompareTypes::Under:			return conditionCheckInfo.Value >  crrValue;
		}
	}
	
	return false;
}

#if WITH_EDITOR

void UBTDecorator_CheckSurvey::BuildDescription()
{
	CachedDescription.Empty();
	CachedDescription.Append(TEXT("Task_Survey가 선행되어야 합니다\n\n"));

	for (int i = 0 ; i < m_ConditionCheckInfos.Num(); ++i)
	{
		const auto& checkInfo = m_ConditionCheckInfos[i];


		CachedDescription.Append(UEnumToString(checkInfo.ConditionType));
		CachedDescription.Append(TEXT(" / "));
		CachedDescription.Append(UEnumToString(checkInfo.CompareType));
		CachedDescription.Append(TEXT(" / "));

		switch (checkInfo.ConditionType)
		{
			case EConditionTypes::Friendly_Highest_HPRate:
			case EConditionTypes::Friendly_Lowest_HPRate:
			case EConditionTypes::Enemy_Highest_HPRate:
			case EConditionTypes::Enemy_Lowest_HPRate:
				CachedDescription.Append(FString::Printf(TEXT("%.1f"), checkInfo.Value * 0.001f));
				break;
			default:
				CachedDescription.Append(FString::Printf(TEXT("%d"), checkInfo.Value));
				break;
		}
		
	}
}

void UBTDecorator_CheckSurvey::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// 변경 사항 시
	Super::PostEditChangeProperty(PropertyChangedEvent);
	BuildDescription();
}

void UBTDecorator_CheckSurvey::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	BuildDescription();
}

#endif