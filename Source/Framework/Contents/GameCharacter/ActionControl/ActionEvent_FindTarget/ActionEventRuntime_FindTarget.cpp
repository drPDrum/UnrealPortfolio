// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventRuntime_FindTarget.h"

#include "Framework/Contents/GameCharacter/ActionControl/ActionController.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"

UActionEventRuntime_FindTarget::UActionEventRuntime_FindTarget()
{
	m_pActionRuntime_ifNotFound = nullptr;
}

UActionEventRuntime_FindTarget::~UActionEventRuntime_FindTarget()
{
}

void UActionEventRuntime_FindTarget::InitLinkedActionRuntime(const TMap<FString, UActionRuntime*>& map_ActionRuntimes)
{
	ActionEventRuntimeBase<FActionEventData_FindTarget>::InitLinkedActionRuntime(map_ActionRuntimes);

	if (!m_pData->m_ifNotFoundPlay.IsEmpty() && map_ActionRuntimes.Contains(m_pData->m_ifNotFoundPlay))
		m_pActionRuntime_ifNotFound = map_ActionRuntimes[m_pData->m_ifNotFoundPlay];
}

bool UActionEventRuntime_FindTarget::OnStart(float ElapsedTime)
{
	// AI 작동중에는 AI가 찾아놨을 것
	if (!m_pOwner->GetBrainCtrl()->GetOrderByUserInput())
		return false;
		
	if (m_pOwner->GetBrainCtrl()->SetTarget(
		m_pData->m_fRadius,
		m_pData->m_TargetType,
		m_pData->m_TargetStateType))
		return false;

	if (m_pActionRuntime_ifNotFound)
	{
		m_pOwner->GetActionCtrl()->PlayAction(m_pActionRuntime_ifNotFound);
		return true;
	}

	return false;
}
