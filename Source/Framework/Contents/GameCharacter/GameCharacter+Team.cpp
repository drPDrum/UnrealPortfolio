#include "GameCharacter.h"
#include "BrainControl/BrainController.h"

GameCharacterAllyInfo* AGameCharacter::AllyInfo = nullptr;

void AGameCharacter::ChangeTeam(const int& idx_Team)
{
	m_pMyTeamInfo = AllyInfo->GetTeamInfo(idx_Team);
	RefreshCollisionProfile();
}

void AGameCharacter::RefreshCollisionProfile()
{
	if (m_pMyTeamInfo)
	{
		auto pCapsuleComp = GetCapsuleComponent();
		if (pCapsuleComp)
		{
			if (m_BrainCtrl->GetOrderByUserInput())
				pCapsuleComp->SetCollisionProfileName(TEXT("GameCharacter_Player"));
			else
				pCapsuleComp->SetCollisionProfileName(*m_pMyTeamInfo->GetCollisionProfile());
		}
	}

	RefreshName();
}

void AGameCharacter::ChangeState(const EGameCharacterStateTypes& state)
{
	if (m_pMyTeamInfo)
	{
		switch (state)
		{
		case EGameCharacterStateTypes::Alive:
		case EGameCharacterStateTypes::Die:
			AllyInfo->GetTeamContainer(m_pMyTeamInfo->GetIdx(), state)->AddTail(m_pNode_State);
			return;
		default:
			break;
		}	
	}

	m_pNode_State->RemoveSelf();
}

const GameCharacterTeamInfo* AGameCharacter::GetTeamInfo() const
{
	return m_pMyTeamInfo;
}
