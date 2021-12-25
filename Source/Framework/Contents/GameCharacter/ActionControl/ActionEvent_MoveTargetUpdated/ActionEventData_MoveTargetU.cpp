// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_MoveTargetU.h"
#include "ActionEventRuntime_MoveTargetU.h"
#include "../ActionController.h"

FActionEventData_MoveTargetU::FActionEventData_MoveTargetU()
{
	m_fMoveSpdPerSec = 200.f;
	
	m_bUseUpdateAngleSpd = false;
	m_fUpdateAngleSpd = 360.f;
	
	m_fOnReachDist = 0.f;
}

FActionEventData_MoveTargetU::~FActionEventData_MoveTargetU()
{
}

CreateRuntimeResult FActionEventData_MoveTargetU::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_MoveTargetU>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_MoveTargetU::GetActionEventType() const
{
	return ActionEventTypes::MoveToTargetUpdated;
}