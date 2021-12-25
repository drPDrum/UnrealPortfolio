// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldManager.h"
#include "Framework/Managers/Managers.h"
#include "Framework/Managers/UIManager/UIBase/LoadingBase.h"
#include "RuntimeUtils/GenericEx/Common.h"
//#include "GameContents/SpawnSystem/SpawnSystem.h"


WorldManager::WorldManager()
{
	m_eWorldID = EWorldId::Intro;
	m_ePrevWorldID = EWorldId::Intro;
	m_pCurWorld = nullptr;
	m_pCurGameMode = nullptr;

	m_bLoadingWorld = false;
	m_pLoadingPage = nullptr;

	m_bLoadingLevel = false;
	m_pCurLandActor = nullptr;
	m_nUUIDLevelLoad = 0;
}

WorldManager::~WorldManager()
{
}

void WorldManager::RegistWorld(UWorld* const pWorld)
{
	m_pCurWorld = pWorld;
	m_pCurGameMode = Cast<AFrameworkGameModeBase>(pWorld->GetAuthGameMode());
}

UWorld* WorldManager::GetWorld() const
{
	return m_pCurWorld;
}

const EWorldId& WorldManager::GetWorldID() const
{
	return m_eWorldID;
}

const EWorldId& WorldManager::GetPrevWorldID() const
{
	return m_ePrevWorldID;
}

const TSet<FString>& WorldManager::GetLoadedLandSet() const
{
	return m_setLoadedLand;
}

AFrameworkGameModeBase* WorldManager::GetGameMode() const
{
	return m_pCurGameMode;
}

const bool& WorldManager::IsLoading() const
{
	return m_bLoadingWorld;
}

void WorldManager::LoadWorld(const EWorldId& eWorldId, const FString& strLoadingName)
{
	if (m_pLoadingPage || m_eWorldID == eWorldId)
		return;

	m_bLoadingWorld = true;
	m_ePrevWorldID = m_eWorldID;

	// 레벨 로드 목록 및 대기열 목록 클리어
	InitLevelLandData(eWorldId);
	
	// 로딩 페이지 오픈
	m_pLoadingPage = Managers::UI->OpenLoading(strLoadingName);

	if (m_pLoadingPage)
		m_pLoadingPage->Open();
	else
		OnEnd_LoadingAnim_Open();
}

// ULoadingBase->OnEndAnim_Open에서 호출됨
void WorldManager::OnEnd_LoadingAnim_Open() const
{
	if(m_pCurGameMode)
		m_pCurGameMode->OnEndGame();

	// 매니저들에게 레벨이 바뀌기 직전임을 알림
	Managers::OnPrevChangeWorld();

	LOG(TEXT("OpenLevel from '%s' to '%s"), *UEnumToString(m_ePrevWorldID), *UEnumToString(m_eWorldID));
	// 레벨을 연다
	UGameplayStatics::OpenLevel(GetWorld(), *FString::Printf(TEXT("Level_%s"), *UEnumToString(m_eWorldID)));
}

void WorldManager::OnStart_GameModeInit(AFrameworkGameModeBase* pGameMode)
{
	m_pCurGameMode = pGameMode;
	m_pCurWorld = pGameMode->GetWorld();

	Managers::OnPostChangeWorld();
}

// ULoadingBase->OnEndAnim_Close에서 호출됨
void WorldManager::OnEnd_GameModeInit()
{
	if (m_pLoadingPage)
		m_pLoadingPage->Close();
	else
		OnEnd_LoadingAnim_Close();
}

void WorldManager::OnEnd_LoadingAnim_Close()
{
	Managers::UI->CloseLoading(m_pLoadingPage);
	m_pLoadingPage = nullptr;
	m_bLoadingWorld = false;

	if (m_pCurGameMode)
		m_pCurGameMode->OnStartGame();
}
