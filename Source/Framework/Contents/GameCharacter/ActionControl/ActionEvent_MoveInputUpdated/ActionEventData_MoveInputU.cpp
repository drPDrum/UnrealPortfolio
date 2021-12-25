// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_MoveInputU.h"
#include "ActionEventRuntime_MoveInputU.h"
#include "../ActionController.h"

FActionEventData_MoveInputU::FActionEventData_MoveInputU()
{
	m_fMoveSpdPerSec = 200.f;
	m_bUseInputScale = false;

	m_bUseUpdateAngleSpd = false;
	m_fUpdateAngleSpd = 360.f;

}

FActionEventData_MoveInputU::~FActionEventData_MoveInputU()
{
}

CreateRuntimeResult FActionEventData_MoveInputU::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_MoveInputU>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_MoveInputU::GetActionEventType() const
{
	return ActionEventTypes::MoveByInputUpdated;
}