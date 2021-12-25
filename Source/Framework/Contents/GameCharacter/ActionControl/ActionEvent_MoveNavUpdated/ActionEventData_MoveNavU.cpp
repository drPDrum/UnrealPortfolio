// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_MoveNavU.h"
#include "ActionEventRuntime_MoveNavU.h"
#include "../ActionController.h"

FActionEventData_MoveNavU::FActionEventData_MoveNavU()
{
	m_bRoateToMoveDir = false;
	m_fMoveSpdPerSec = 200.f;
}

FActionEventData_MoveNavU::~FActionEventData_MoveNavU()
{
}

CreateRuntimeResult FActionEventData_MoveNavU::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_MoveNavU>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_MoveNavU::GetActionEventType() const
{
	return ActionEventTypes::MoveNavUpdated;
}