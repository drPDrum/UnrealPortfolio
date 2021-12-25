// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "Framework/Managers/IManager.h"
#include "Framework/UEBaseClass/FrameworkGameModeBase.h"

enum class EWorldId : uint8;
class ULoadingBase;
class CSpawnData;

class FRAMEWORK_API WorldManager : public IManagers
{
private:
	enum class ELevelExeType : uint8
	{
		None,
		LoadLand,
		UnloadLand,
	};
	
	struct SLevelExeInfo
	{
		ELevelExeType	ExeType;
		FString			LevelName;

		SLevelExeInfo();
		SLevelExeInfo(const ELevelExeType& eExeType, const FString& strLevelName);
	};

	// For World
	EWorldId							m_eWorldID;
	EWorldId							m_ePrevWorldID;
	UWorld*								m_pCurWorld;
	AFrameworkGameModeBase*				m_pCurGameMode;
										
	bool								m_bLoadingWorld;
	ULoadingBase*						m_pLoadingPage;
										
	// For Level
	TMap<FString, TSet<FString>>		m_mapLevelLandData;	//레벨 랜드 데이터
	// TMap<FString, TArray<CSpawnData*>>	m_mapSpawnData;
	bool								m_bLoadingLevel;	// 레벨 로드 중인가?
	TSet<FString>						m_setLoadedLand;	// 로드 완료된 랜드
										
	AActor*								m_pCurLandActor;
										
	TQueue<SLevelExeInfo>				m_qLevelExecuteInfo;// 로드 대기 큐
										
	int32								m_nUUIDLevelLoad;
	std::function<void()>				m_onLoadLandComplete;

	
public:
	WorldManager();
	virtual ~WorldManager() override;

	void RegistWorld(UWorld* const pWorld);

	UWorld* GetWorld() const;
	const EWorldId& GetWorldID() const;
	const EWorldId& GetPrevWorldID() const;
	const TSet<FString>& GetLoadedLandSet() const;
	AFrameworkGameModeBase* GetGameMode() const;
	const bool& IsLoading() const;

	void LoadWorld(const EWorldId& eWorldId, const FString& strLoadingName);

public: // GameMode 및 LoadinUI 통신
	void OnEnd_LoadingAnim_Open() const;
	void OnEnd_LoadingAnim_Close();

	void OnStart_GameModeInit(AFrameworkGameModeBase* pGameMode);
	void OnEnd_GameModeInit();

public:	// 레벨 스트리밍용 +LevelStreaming.cpp
	void InitLevelLandData(const EWorldId& eWorldId);
	
	const bool& IsLevelLoading() const;

	void LoadLandAsync(const FString&  strLandName);
	void SetLevelAsyncEndCallback(std::function<void()> callback);
	void LoadCompleteLevel();

	// void GetSpawnData(TArray<CSpawnData*>& arrLoadSpawnData);

	void CheckAndLoadLand(const AActor* const& target);
};