// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionEventData_Animation.h"
#include "ActionEventRuntime_Animation.h"
#include "../ActionController.h"

FActionEventData_Animation::FActionEventData_Animation()
{
	m_fFadeTime = 0.3f;
	m_fPlayRate = 1.f;
	m_bResetIfPlaying = false;
	m_BlendOption = EAnimBlendOption::None;
}

FActionEventData_Animation::~FActionEventData_Animation()
{
}

CreateRuntimeResult FActionEventData_Animation::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_Animation>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_Animation::GetActionEventType() const
{
	return ActionEventTypes::Animation;
}
