#include "BrainController.h"
#include "../GameCharacter.h"
#include "../ActionControl/ActionController.h"

void ABrainController::SetOrderByUserInput(const bool& bEnable)
{
	m_IsOrderByUserInput = bEnable;
	if (bEnable)
		StopAI();
	else
		StartAI();
}

const bool& ABrainController::GetOrderByUserInput() const
{
	return m_IsOrderByUserInput;
}

void ABrainController::OnInput_MoveAxisX(const float& scale)
{
	m_bRefreshInput = m_bRefreshInput || m_vInputScaleMove.X != scale;
	m_vInputScaleMove.X = scale;
}

void ABrainController::OnInput_MoveAxisY(const float& scale)
{
	m_bRefreshInput = m_bRefreshInput || m_vInputScaleMove.Y != scale;
	m_vInputScaleMove.Y = scale;
}

void ABrainController::OnInput_Key(const ETriggerEventTypes& keyType)
{
	if (m_pOwner)
	{
		const auto& pActionCtrl = m_pOwner->GetActionCtrl();
		const auto& pNextActionRuntime = pActionCtrl->CheckNextAction(keyType);
		
		pActionCtrl->PlayAction(pNextActionRuntime);
	}
}