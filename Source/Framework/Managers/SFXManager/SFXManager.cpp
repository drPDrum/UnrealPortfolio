// Fill out your copyright notice in the Description page of Project Settings.


#include "SFXManager.h"

#include "SFXActor.h"
#include "../Managers.h"

SFXManager::SFXManager()
{
}

SFXManager::~SFXManager()
{
	// 활성화된 엑터 삭제
	auto node_Active = m_llist_Active.GetHead();
	while (node_Active)
	{
		auto node_Delete = node_Active;
		node_Active = node_Active->GetNextNode();
		node_Delete->RemoveSelf();

		auto pFxActor = node_Delete->GetValue();
		if (pFxActor && IsValid(pFxActor))
			pFxActor->Destroy();
	}

	// 비활성화된 엑터 삭제
	auto node_Inactive = m_llist_Inactive.GetHead();
	while (node_Inactive)
	{
		auto node_Delete = node_Inactive;
		node_Inactive = node_Inactive->GetNextNode();
		node_Delete->RemoveSelf();

		auto pFxActor = node_Delete->GetValue();
		if (pFxActor && IsValid(pFxActor))
			pFxActor->Destroy();
	}
}

bool SFXManager::Init()
{
	if (m_llist_Inactive.Num() != 0)
		return true;
	
	// Spawn SFXActor Contatiner
	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int i = 0 ; i < 30; ++i)
	{
		auto sfxActor = Managers::World->GetWorld()->SpawnActor<ASFXActor>(param);
		sfxActor->ReturnToManager();

#if WITH_EDITOR
		sfxActor->SetFolderPath(TEXT("Actors_SFX"));
#endif
	}

	return true;
}

void SFXManager::OnPrevChangeLevel()
{
	// 모든 액터가 파괴 될 것, 회수 및 리스트 초기화
	const auto& node_Sfx = m_llist_Active.GetHead();
	while (node_Sfx)
	{
		const auto& pSfxActor = node_Sfx->GetValue();
		node_Sfx->GetNextNode();
		pSfxActor->ReturnToManager();
	}

	m_llist_Inactive.Empty(false);
}

void SFXManager::OnPostChangeLevel()
{
	// 다시 Init
	Init();
}


ASFXActor* SFXManager::PlaySFX(USoundBase* sound, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation)
{
	auto pSfxActor = GenerateSFX();
	pSfxActor->Play(sound, parent, socketName, bFollowParent, location, rotation);
	return pSfxActor;
}

ASFXActor* SFXManager::PlaySFX(USoundBase* sound, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FOnSFXReturn& onReturn)
{
	auto pSfxActor = GenerateSFX();
	pSfxActor->Play(sound, parent, socketName, bFollowParent, location, rotation, onReturn);
	return pSfxActor;
}

ASFXActor* SFXManager::PlaySFX(USoundBase* sound, const FVector& location, const FQuat& rotation)
{
	return PlaySFX(sound, nullptr, FName(TEXT("None")), false, location, rotation);
}

ASFXActor* SFXManager::PlaySFX(USoundBase* sound, const FVector& location, const FQuat& rotation, const FOnSFXReturn& onReturn)
{
	return PlaySFX(sound, nullptr, FName(TEXT("None")), false, location, rotation, onReturn);
}

void SFXManager::RetrieveSFX(ASFXActor* sfxActor)
{
	if (sfxActor)
		m_llist_Inactive.AddTail(sfxActor->GetNode());
}

ASFXActor* SFXManager::GenerateSFX()
{
	auto node_SfxActor = m_llist_Inactive.GetHead();
	if (!node_SfxActor)
	{
		node_SfxActor = m_llist_Active.GetHead();
		node_SfxActor->RemoveSelf();
	}

	m_llist_Active.AddTail(node_SfxActor);
	return node_SfxActor->GetValue();
}
