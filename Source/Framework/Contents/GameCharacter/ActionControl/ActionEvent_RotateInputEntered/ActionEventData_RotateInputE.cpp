#include "ActionEventData_RotateInputE.h"
#include "ActionEventRuntime_RotateInputE.h"
#include "../../ActionControl/ActionController.h"

FActionEventData_RotateInputE::FActionEventData_RotateInputE()
{
	m_fRotateSpdPerSec = 360.f;
}

FActionEventData_RotateInputE::~FActionEventData_RotateInputE()
{
}

CreateRuntimeResult FActionEventData_RotateInputE::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_RotateInputE>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_RotateInputE::GetActionEventType() const
{
	return ActionEventTypes::RotateByInputEntered;
}