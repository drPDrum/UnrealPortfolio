// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_MoveNavE.h"
#include "ActionEventRuntime_MoveNavE.h"
#include "../ActionController.h"

FActionEventData_MoveNavE::FActionEventData_MoveNavE()
{
	m_fMoveSpdPerSec = 200.f;
}

FActionEventData_MoveNavE::~FActionEventData_MoveNavE()
{
}

CreateRuntimeResult FActionEventData_MoveNavE::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_MoveNavE>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_MoveNavE::GetActionEventType() const
{
	return ActionEventTypes::MoveNavEntered;
}