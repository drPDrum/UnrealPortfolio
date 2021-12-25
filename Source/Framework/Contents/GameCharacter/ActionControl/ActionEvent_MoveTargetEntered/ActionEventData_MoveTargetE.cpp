// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_MoveTargetE.h"
#include "ActionEventRuntime_MoveTargetE.h"
#include "../ActionController.h"

FActionEventData_MoveTargetE::FActionEventData_MoveTargetE()
{
	m_fMoveSpdPerSec = 200.f;
	m_fOnReachDist = 0.f;
}

FActionEventData_MoveTargetE::~FActionEventData_MoveTargetE()
{
}

CreateRuntimeResult FActionEventData_MoveTargetE::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_MoveTargetE>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_MoveTargetE::GetActionEventType() const
{
	return ActionEventTypes::MoveToTargetEntered;
}