// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "FXActor.h"
#include "Framework/Managers/IManager.h"

class FXContainer
{
	friend class FXManager;
private:
	UClass* m_StaticClass;
	TDoubleLinkedListEx<AFXActor*> m_llist_Active;
	TDoubleLinkedListEx<AFXActor*> m_llist_Inactive;

	int m_nRefCount;
	bool m_bIsLoop;

public:
	FXContainer();
	~FXContainer();

	const bool& IsLooping() const;
};

class FRAMEWORK_API FXManager : public IManagers
{
private:
	TMap<FString, FXContainer*> m_map_FXContainer;

public:
	FXManager();
	virtual ~FXManager() override;

public:
	void OnPrevChangeLevel();

	AFXActor* PlayFX(FString FxName, const FVector& location, const FQuat& rotation, const FVector& scale);
	AFXActor* PlayFX(FString FxName, const FVector& location, const FQuat& rotation, const FVector& scale, const FOnFXReturn& onReturn);
	AFXActor* PlayFX(FString FxName, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale);
	AFXActor* PlayFX(FString FxName, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale, const FOnFXReturn& onReturn);
	
	AFXActor* PlayFX(FXContainer* fxContainer, const FVector& location, const FQuat& rotation, const FVector& scale);
	AFXActor* PlayFX(FXContainer* fxContainer, const FVector& location, const FQuat& rotation, const FVector& scale, const FOnFXReturn& onReturn);
	AFXActor* PlayFX(FXContainer* fxContainer, AActor* parent, const FName& socketName,  const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale);
	AFXActor* PlayFX(FXContainer* fxContainer, AActor* parent, const FName& socketName,  const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale, const FOnFXReturn& onReturn);
	void RetrieveFX(AFXActor* fxActor);

	FXContainer* RegistFX(UClass* fxClass, int nDefaultCount);
	void ReleaseFX(FString fxName);
	void ReleaseFX(FXContainer* fxName);

private:
	AFXActor* GenerateFX(FXContainer* const fxContainer);
};