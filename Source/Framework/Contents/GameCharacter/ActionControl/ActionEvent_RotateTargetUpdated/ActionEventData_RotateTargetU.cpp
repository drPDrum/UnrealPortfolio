#include "ActionEventData_RotateTargetU.h"
#include "ActionEventRuntime_RotateTargetU.h"
#include "../../ActionControl/ActionController.h"

FActionEventData_RotateTargetU::FActionEventData_RotateTargetU()
{
	m_fRotateSpdPerSec = 360.f;
}

FActionEventData_RotateTargetU::~FActionEventData_RotateTargetU()
{
}

CreateRuntimeResult FActionEventData_RotateTargetU::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_RotateTargetU>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_RotateTargetU::GetActionEventType() const
{
	return ActionEventTypes::RotateByInputUpdated;
}