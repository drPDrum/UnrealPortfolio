// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_PlaySFX.h"
#include "ActionEventRuntime_PlaySFX.h"
#include "../ActionController.h"

FActionEventData_PlaySFX::FActionEventData_PlaySFX()
{
	m_vLocation = FVector::ZeroVector;
	m_qRotation = FQuat::Identity;
	m_bIsFollow = false;
}

FActionEventData_PlaySFX::~FActionEventData_PlaySFX()
{
}

CreateRuntimeResult FActionEventData_PlaySFX::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_PlaySFX>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_PlaySFX::GetActionEventType() const
{
	return ActionEventTypes::PlaySFX;
}