#include "ActionEventData_RotateVel.h"
#include "ActionEventRuntime_RotateVel.h"
#include "../../ActionControl/ActionController.h"

FActionEventData_RotateVel::FActionEventData_RotateVel()
{
	m_fRotateSpdPerSec = 360.f;
}

FActionEventData_RotateVel::~FActionEventData_RotateVel()
{
}

CreateRuntimeResult FActionEventData_RotateVel::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_RotateVel>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_RotateVel::GetActionEventType() const
{
	return ActionEventTypes::RotateToVelocity;
}