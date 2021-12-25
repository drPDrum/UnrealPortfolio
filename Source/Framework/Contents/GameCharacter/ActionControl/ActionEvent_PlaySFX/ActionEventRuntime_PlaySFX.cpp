#include "ActionEventRuntime_PlaySFX.h"

#include "Framework/Managers/Managers.h"
#include "Framework/Managers/SFXManager/SFXActor.h"
#include "Framework/Managers/SFXManager/SFXManager.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionController.h"

UActionEventRuntime_PlaySFX::UActionEventRuntime_PlaySFX()
{
	m_pSoundBase = nullptr;
}

UActionEventRuntime_PlaySFX::~UActionEventRuntime_PlaySFX()
{
	m_pSoundBase = nullptr;

	if (m_pSFXActor)
		m_pSFXActor->ReturnToManager();
}

void UActionEventRuntime_PlaySFX::Init(FActionEventData_PlaySFX& Data, AGameCharacter* Owner)
{
	ActionEventRuntimeBase<FActionEventData_PlaySFX>::Init(Data, Owner);

	m_pSoundBase = Managers::Asset->LoadObj<USoundBase>(*m_pData->m_ResPath_SFX);
}


bool UActionEventRuntime_PlaySFX::OnStart(float elapsedTime)
{
	if (m_pSoundBase)
	{
		if (m_pSoundBase->IsLooping())
		{
			if (!m_pSFXActor)
			{
				FOnSFXReturn returnCallback;
				returnCallback.BindDynamic(this, &UActionEventRuntime_PlaySFX::OnReturnSFX);
			
				m_pSFXActor = Managers::SFX->PlaySFX(
					m_pSoundBase,
					m_pOwner,
					m_pData->m_SocketName,
					m_pData->m_bIsFollow,
					m_pData->m_vLocation,
					m_pData->m_qRotation,
					returnCallback
					);

				m_ActionCtrl->AddLoopingSFX(m_pSFXActor);	
			}
		}
		else
		{
			Managers::SFX->PlaySFX(
				m_pSoundBase,
				m_pOwner,
				m_pData->m_SocketName,
				m_pData->m_bIsFollow,
				m_pData->m_vLocation,
				m_pData->m_qRotation
				);
		}
	}
	
	return false;
}

void UActionEventRuntime_PlaySFX::OnReturnSFX(ASFXActor* pSFXReturned)
{
	if (m_pSFXActor == pSFXReturned)
	{
		m_ActionCtrl->RemoveLoopingSFX(m_pSFXActor);
		m_pSFXActor = nullptr;
	}
}