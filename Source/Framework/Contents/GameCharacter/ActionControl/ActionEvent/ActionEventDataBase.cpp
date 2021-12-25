// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventDataBase.h"

FActionEventDataBase::FActionEventDataBase()
{
	fStartTime = 0.f;
}

FActionEventDataBase::~FActionEventDataBase()
{
}

CreateRuntimeResult FActionEventDataBase::CreateRuntime(AGameCharacter* owner)
{
	CreateRuntimeResult result;

	result.pUObject = nullptr;
	result.pRuntime = nullptr;

	return result;
}

ActionEventTypes FActionEventDataBase::GetActionEventType() const
{
	return ActionEventTypes::None;
}

bool FActionEventDataBase::IsDuration() const
{
	return false;
}
