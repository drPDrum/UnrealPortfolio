#include "ActionEventData_RotateTargetE.h"
#include "ActionEventRuntime_RotateTargetE.h"
#include "../../ActionControl/ActionController.h"

FActionEventData_RotateTargetE::FActionEventData_RotateTargetE()
{
	m_fRotateSpdPerSec = 360.f;
}

FActionEventData_RotateTargetE::~FActionEventData_RotateTargetE()
{
	
}

CreateRuntimeResult FActionEventData_RotateTargetE::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_RotateTargetE>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_RotateTargetE::GetActionEventType() const
{
	return ActionEventTypes::RotateToTargetEntered;
}