#include "ActionEventData_RotateInputU.h"
#include "ActionEventRuntime_RotateInputU.h"
#include "../../ActionControl/ActionController.h"


FActionEventData_RotateInputU::FActionEventData_RotateInputU()
{
	m_fRotateSpdPerSec = 360.f;
}

FActionEventData_RotateInputU::~FActionEventData_RotateInputU()
{
}

CreateRuntimeResult FActionEventData_RotateInputU::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_RotateInputU>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_RotateInputU::GetActionEventType() const
{
	return ActionEventTypes::RotateByInputUpdated;
}