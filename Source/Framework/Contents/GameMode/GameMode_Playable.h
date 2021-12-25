// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Framework/Contents/Spawn/SpawnPoint.h"
#include "Framework/UEBaseClass/FrameworkGameModeBase.h"
#include "GameMode_Playable.generated.h"

class AGameCharacter;
class CUserCharData;

UCLASS()
class FRAMEWORK_API AGameMode_Playable : public AFrameworkGameModeBase
{
	GENERATED_BODY()

protected:
	bool								m_bInit;
	ASpawnPoint*						m_pCharSpawnPoint;
	TMap<FString, TArray<ASpawnPoint*>>	m_mapMonsterSpawnPoint;
	
public:
	AGameMode_Playable();

	virtual void OnInit() override;
	virtual void OnLevelLoaded();
	virtual void OnInit_CharacterLoad();
	virtual void OnMonsterSpawnCheck();
	virtual void OnInit_AfterLevelLoaded();	
	virtual void OnStartGame() override;
	virtual void OnEndGame() override;
};