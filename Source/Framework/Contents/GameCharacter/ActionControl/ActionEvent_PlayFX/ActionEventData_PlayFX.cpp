// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_PlayFX.h"
#include "ActionEventRuntime_PlayFX.h"
#include "../ActionController.h"

FActionEventData_PlayFX::FActionEventData_PlayFX()
{
	m_vLocation = FVector::ZeroVector;
	m_qRotation = FQuat::Identity;
	m_vScale = FVector::OneVector;
	m_bIsFollow = false;
}

FActionEventData_PlayFX::~FActionEventData_PlayFX()
{
}

CreateRuntimeResult FActionEventData_PlayFX::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_PlayFX>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_PlayFX::GetActionEventType() const
{
	return ActionEventTypes::PlayFX;
}
