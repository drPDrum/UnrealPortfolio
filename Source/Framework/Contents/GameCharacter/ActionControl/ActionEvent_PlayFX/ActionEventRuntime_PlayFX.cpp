#include "ActionEventRuntime_PlayFX.h"

#include "Framework/Managers/Managers.h"
#include "Framework/Managers/FXManager/FXActor.h"
#include "Framework/Managers/FXManager/FXManager.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionController.h"

UActionEventRuntime_PlayFX::UActionEventRuntime_PlayFX()
{
	m_pFXContainer = nullptr;
}

UActionEventRuntime_PlayFX::~UActionEventRuntime_PlayFX()
{
	if (m_pFXContainer && Managers::IsInit())
		Managers::FX->ReleaseFX(m_pFXContainer);

	m_pFXContainer = nullptr;
}

void UActionEventRuntime_PlayFX::Init(FActionEventData_PlayFX& Data, AGameCharacter* Owner)
{
	ActionEventRuntimeBase<FActionEventData_PlayFX>::Init(Data, Owner);

	auto BPClass = Managers::Asset->LoadBP<AFXActor>(*(m_pData->m_ResPath_FX));
	if (BPClass)
		m_pFXContainer = Managers::FX->RegistFX(BPClass, 5);
	else
		m_pFXContainer = nullptr;
}

bool UActionEventRuntime_PlayFX::OnStart(float elapsedTime)
{
	if (!m_pFXContainer)
		return false;
	
	if (m_pFXContainer->IsLooping())
	{
		if (!m_pFXActor)
		{
			FOnFXReturn returnCallback;
			returnCallback.BindDynamic(this, &UActionEventRuntime_PlayFX::OnReturnFX);
			
			m_pFXActor = Managers::FX->PlayFX(
				m_pFXContainer,
				m_pOwner,
				m_pData->m_SocketName,
				m_pData->m_bIsFollow,
				m_pData->m_vLocation,
				m_pData->m_qRotation,
				m_pData->m_vScale,
				returnCallback
				);

			m_ActionCtrl->AddLoopingFX(m_pFXActor);

			if (m_pFXActor && m_pFXActor->HasGhost())
			{
				// Todo
				TArray<USkeletalMeshComponent*> meshComp;
				meshComp.Add(m_pOwner->GetMesh());
				m_pFXActor->SetGhost(meshComp);
			}
		}
	}
	else
	{
		auto pFXActor =  Managers::FX->PlayFX(
			m_pFXContainer,
			m_pOwner,
			m_pData->m_SocketName,
			m_pData->m_bIsFollow,
			m_pData->m_vLocation,
			m_pData->m_qRotation,
			m_pData->m_vScale
			);
			
		if (pFXActor && pFXActor->HasGhost())
		{
			// Todo
			TArray<USkeletalMeshComponent*> meshComp;
			meshComp.Add(m_pOwner->GetMesh());
			pFXActor->SetGhost(meshComp);
		}
	}
	
	return false;
}

void UActionEventRuntime_PlayFX::OnReturnFX(AFXActor* pSFXReturned)
{
	if (m_pFXActor == pSFXReturned)
	{
		m_ActionCtrl->RemoveLoopingFX(m_pFXActor);
		m_pFXActor = nullptr;
	}
}