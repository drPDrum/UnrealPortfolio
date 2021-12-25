// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_FindTarget.h"
#include "ActionEventRuntime_FindTarget.h"
#include "../ActionController.h"

FActionEventData_FindTarget::FActionEventData_FindTarget()
{
	m_fRadius = 1000.f;
	m_TargetType = EGameCharacterFindTargetTypes::Enemy;
	m_TargetStateType = EGameCharacterStateTypes::Alive;
}

FActionEventData_FindTarget::~FActionEventData_FindTarget()
{
}

CreateRuntimeResult FActionEventData_FindTarget::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_FindTarget>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_FindTarget::GetActionEventType() const
{
	return ActionEventTypes::FindTarget;
}
