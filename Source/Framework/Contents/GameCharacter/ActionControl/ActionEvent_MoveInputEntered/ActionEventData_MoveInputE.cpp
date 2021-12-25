// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_MoveInputE.h"
#include "ActionEventRuntime_MoveInputE.h"
#include "../ActionController.h"

FActionEventData_MoveInputE::FActionEventData_MoveInputE()
{
	m_fMoveSpdPerSec = 200.f;
}

FActionEventData_MoveInputE::~FActionEventData_MoveInputE()
{
}

CreateRuntimeResult FActionEventData_MoveInputE::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_MoveInputE>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_MoveInputE::GetActionEventType() const
{
	return ActionEventTypes::MoveByInputEntered;
}