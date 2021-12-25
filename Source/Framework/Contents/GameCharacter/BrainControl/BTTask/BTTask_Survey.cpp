// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Survey.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "Framework/Contents/GameCharacter/BrainControl/SurveyInfo.h"

UBTTask_Survey::UBTTask_Survey()
{
	bNotifyTick = false;
}

UBTTask_Survey::~UBTTask_Survey()
{
}

EBTNodeResult::Type UBTTask_Survey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto pAICtrl = Cast<ABrainController>(OwnerComp.GetAIOwner());
	if (!pAICtrl)
		return EBTNodeResult::Succeeded;

  	pAICtrl->SetSurveyInfo();

	auto pSurveyInfo = pAICtrl->GetSurveyInfo();
	if (!pSurveyInfo)
		return EBTNodeResult::Succeeded;

	// 적 캐릭터가 한마리도 없다면 기본 행동으로 변경
	if (pSurveyInfo->nCount_Enemy_Alive == 0)
	{
		// Todo :: 주변 동료 전투 중 체크
		
		pAICtrl->SetCurrAIBehavior(static_cast<EAIBehaviorTypes>(pAICtrl->GetDefaultAIBehavior()));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}