// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_Rotate.h"
#include "ActionEventRuntime_Rotate.h"
#include "../ActionController.h"

FActionEventData_Rotate::FActionEventData_Rotate()
{
}

FActionEventData_Rotate::~FActionEventData_Rotate()
{
}

CreateRuntimeResult FActionEventData_Rotate::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_Rotate>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_Rotate::GetActionEventType() const
{
	return ActionEventTypes::Rotate;
}