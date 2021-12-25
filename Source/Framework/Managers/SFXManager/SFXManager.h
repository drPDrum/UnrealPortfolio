// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SFXActor.h"
#include "Framework/Managers/IManager.h"

class FRAMEWORK_API SFXManager : IManagers
{
private:
	TDoubleLinkedListEx<ASFXActor*> m_llist_Active;
	TDoubleLinkedListEx<ASFXActor*> m_llist_Inactive;
	int m_Test = 0;

public:
	SFXManager();
	~SFXManager();

	virtual bool Init() override;
	void OnPrevChangeLevel();
	void OnPostChangeLevel();

	ASFXActor* PlaySFX(USoundBase* sound, const FVector& location, const FQuat& rotation);
	ASFXActor* PlaySFX(USoundBase* sound, const FVector& location, const FQuat& rotation, const FOnSFXReturn& onReturn);
	ASFXActor* PlaySFX(USoundBase* sound, AActor* parent, const FName& socketName, const bool& bFollowParent,  const FVector& location, const FQuat& rotation);
	ASFXActor* PlaySFX(USoundBase* sound, AActor* parent, const FName& socketName, const bool& bFollowParent,  const FVector& location, const FQuat& rotation, const FOnSFXReturn& onReturn);
	void RetrieveSFX(ASFXActor* sfxActor);

private:
	ASFXActor* GenerateSFX();
};
