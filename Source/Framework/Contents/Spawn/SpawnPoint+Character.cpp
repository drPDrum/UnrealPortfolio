#include "SpawnPoint.h"
#include "Components/SplineComponent.h"
#include "Framework/Contents/GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionController.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "Framework/UserInfo/UserInfo.h"
#include "Framework/UserInfo/Character/UserCharacterInfo.h"

bool ASpawnPoint::HasPatrol() const
{
	return m_ePatrolType != EAIDefaultBehaviorType::None && m_pSplineComp->GetSplineLength() > 0.f;
}

AGameCharacter* ASpawnPoint::SpawnCharacter()
{
	UWorld* pWorld = GetWorld();

	if(m_pSpawnedCharacter)
		UnSpawnCharacter();

	CUserCharData* pUserData = nullptr;
	if(m_eSpawnType == ESpawnObjectType::Player)
		pUserData = UserInfo::Character->GetCurUserCharacter();
	else if(m_eSpawnType == ESpawnObjectType::Monster && m_nMonsterCharDataID > 0)
		pUserData = UserInfo::Character->MakeNewCharacter(m_nMonsterCharDataID);

	if(!pUserData)
	{
		LOG_ERROR(TEXT("SpawnCharacter ERROR. UserData == NULL"));
		return nullptr;
	}

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	m_pSpawnedCharacter = GetWorld()->SpawnActor<AGameCharacter>(
		GetActorLocation(),
		GetActorRotation(),
		param);

	m_pSpawnedCharacter->Init(pUserData);

	switch (m_eSpawnType)
	{
		case ESpawnObjectType::Player:
			m_pSpawnedCharacter->ChangeTeam(0);
#if WITH_EDITOR
			m_pSpawnedCharacter->SetFolderPath(*FString::Printf(TEXT("Spawn/Player")));
#endif
			break;
		case ESpawnObjectType::Monster:
			m_pSpawnedCharacter->ChangeTeam(1);
			m_pSpawnedCharacter->GetBrainCtrl()->SetSpawnInfo(
				GetActorLocation(),
				GetActorQuat(),
				m_pSplineComp);
			m_pSpawnedCharacter->GetBrainCtrl()->SetDefaultAIBehavior(m_ePatrolType);

#if WITH_EDITOR
			m_pSpawnedCharacter->SetFolderPath(*FString::Printf(TEXT("Spawn/Monster/%s"), *GetLandName()));
#endif
			break;
	}
	
	m_pSpawnedCharacter->GetActionCtrl()->PlayAction("ActionData_Idle");
	m_pSpawnedCharacter->SetActive(false);
	m_pSpawnedCharacter->GetBrainCtrl()->StopAI();


	
	return m_pSpawnedCharacter;
}

void ASpawnPoint::UnSpawnCharacter()
{
	if(IsValid(m_pSpawnedCharacter))
	{
		GetWorld()->DestroyActor(m_pSpawnedCharacter);
		m_pSpawnedCharacter = nullptr;
	}
}

void ASpawnPoint::SetActive_Character(bool bEnable, bool bControlAI)
{
	m_bCharacterActive = bEnable;
	
	if(!m_pSpawnedCharacter)
		return;

	if(m_pSpawnedCharacter->IsActive() == bEnable)
		return;

	m_pSpawnedCharacter->SetActive(bEnable);

	if(m_eSpawnType == ESpawnObjectType::Monster)
	{
		m_pSpawnedCharacter->SetNavLocation(GetActorLocation());

		auto& pBrain = m_pSpawnedCharacter->GetBrainCtrl();		

		if(bEnable)
		{
			if(bControlAI)
				pBrain->StartAI();	
		}
		else
		{
			if(bControlAI)
				pBrain->StopAI();
		}
	}
}

bool ASpawnPoint::IsActive_Character() const
{
	return m_bCharacterActive;
}

void ASpawnPoint::OnStart_Character()
{
	if(!m_pSpawnedCharacter || !m_bCharacterActive)
		return;

	if(m_eSpawnType == ESpawnObjectType::Player)
		m_pSpawnedCharacter->SetActive(true);
	else if(m_eSpawnType == ESpawnObjectType::Monster)
		m_pSpawnedCharacter->GetBrainCtrl()->StartAI();
}


