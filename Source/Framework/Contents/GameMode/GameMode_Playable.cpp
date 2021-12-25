#include "Gamemode_Playable.h"

#include "Framework/Contents/GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/InputControl/GameCharInputController.h"
#include "Framework/Contents/Spawn/SpawnPoint.h"
#include "Framework/Managers/Managers.h"
#include "Framework/UEBaseClass/FrameworkCamera.h"
#include "RuntimeUtils/GenericEx/Common.h"

class UWindow_GameMain;

AGameMode_Playable::AGameMode_Playable() : AFrameworkGameModeBase()
{
	PlayerControllerClass = AGameCharInputController::StaticClass();

	m_pCharSpawnPoint = nullptr;
	m_bInit = false;
}

void AGameMode_Playable::OnInit()
{
	//모든 UI 제거
	m_bInit = false;
	m_pCharSpawnPoint = nullptr;
	Managers::UI->ClearWindow(false);

	TArray<AActor*> arrSpawnPoint;
	TArray<ASpawnPoint*> arrPlayerSpawnPoint;
	auto& ePrevWorldID = Managers::World->GetPrevWorldID();

	// Level에 배치되어있는 모든 SpawnPoint 데이터화 하기
	UGameplayStatics::GetAllActorsOfClass(Managers::World->GetWorld(), ASpawnPoint::StaticClass(), arrSpawnPoint);
	for (auto& pActor : arrSpawnPoint)
	{
		auto pSpawnSystem = Cast<ASpawnPoint>(pActor);
		if(!pSpawnSystem)
			continue;;

		//Log
		LOG_WARNING(TEXT("Spawn : LandName '%s' WorldID '%s' Pos '%s' Rot '%s'"),
			*pSpawnSystem->GetLandName(),
			*UEnumToString(pSpawnSystem->GetPrevWorldID()),
			*pSpawnSystem->GetActorLocation().ToString(),
			*pSpawnSystem->GetActorRotation().ToString());

		if(pSpawnSystem->GetPrevWorldID() != ePrevWorldID
			&& pSpawnSystem->GetPrevWorldID() != EWorldId::All)
			continue;

		auto& eSpawnObjectType = pSpawnSystem->GetSpawnObjectType();

		switch (eSpawnObjectType)
		{
			case ESpawnObjectType::Player:
				arrPlayerSpawnPoint.Add(pSpawnSystem);
				break;
			case ESpawnObjectType::Monster:
				{
					auto strLandName = pSpawnSystem->GetLandName();
					if(strLandName.IsEmpty())
						continue;

					if(!m_mapMonsterSpawnPoint.Contains(strLandName))
						m_mapMonsterSpawnPoint.Add(strLandName, TArray<ASpawnPoint*>());
			
					m_mapMonsterSpawnPoint[strLandName].Add(pSpawnSystem);
				}
				break;
			default:
				//TODO : no character Objects etc...
				break;
		}
	}

	// 메인 케릭터 스폰 데이터 처리
	if(arrPlayerSpawnPoint.Num() < 1)
	{
		LOG_ERROR(TEXT("Player SpawnPoint ERROR"));
		return;
	}

	if(arrPlayerSpawnPoint.Num() > 1)
		m_pCharSpawnPoint = arrPlayerSpawnPoint[FMath::RandRange(0, arrPlayerSpawnPoint.Num() - 1)];
	else
		m_pCharSpawnPoint = arrPlayerSpawnPoint[0];
	
	if(m_pCharSpawnPoint->GetLandName().IsEmpty())
	{
		LOG_ERROR(TEXT("Player SpawnData ERROR"));
		return;
	}

	// 메인 케릭터 스폰 랜드 기준으로 랜드 레벨 스폰
	Managers::World->LoadLandAsync(m_pCharSpawnPoint->GetLandName());
	// 랜드 레벨 로드 Callback 등록
	Managers::World->SetLevelAsyncEndCallback([this]() { OnLevelLoaded(); });	
}

void AGameMode_Playable::OnLevelLoaded()
{	
	// 현재 초기화 진행 중이면 초기화 함수 호출해 준다.
	if(!m_bInit)
		OnInit_CharacterLoad();

	// 열려있는 랜드 기준으로 몬스터 스폰 체크
	OnMonsterSpawnCheck();

	// 초기화 진행 중이면, 초기화 마무리
	if(!m_bInit)
	{
		OnInit_AfterLevelLoaded();
		m_bInit = true;
	}
}

void AGameMode_Playable::OnInit_CharacterLoad()
{
	// 팀 정보 세팅
	TArray<GameCharacterTeamInfo*> teamInfos = {
		new GameCharacterTeamInfo(0, {0}, {1}),
		new GameCharacterTeamInfo(1, {1}, {0}),
	};
	AGameCharacter::AllyInfo = new GameCharacterAllyInfo(teamInfos, 0);

	UWorld* pWorld = GetWorld();
	
	if(m_pCharSpawnPoint->GetLandName().IsEmpty())
	{
		LOG_ERROR(TEXT("Player SpawnData ERROR"));
		return;
	}
	
	// player spawn
	auto pPlayer = m_pCharSpawnPoint->SpawnCharacter();

	FActorSpawnParameters param;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	// Camera
	auto gameCam = pWorld->SpawnActor<AFrameworkCamera>(
		pPlayer->GetActorLocation(),
		pPlayer->GetActorRotation(),
		param);

	gameCam->SetTarget(pPlayer);

	// UserInput에다 생성된 캐릭터 등록
	auto& pInputCtrl = Managers::UI->GetInputCtrl();
	if (pInputCtrl)
	{
		pInputCtrl->AddKey_PlayCtrl(EUserInputTypes::KeyboardAndMouse, true, true);
		pInputCtrl->Possess(pPlayer);
		pInputCtrl->SetCamera(gameCam);
	}

	// All monster spawn
	for(auto& tuple : m_mapMonsterSpawnPoint)
	{
		for(auto& pSpawnPoint : tuple.Value)
		{
			pSpawnPoint->SpawnCharacter();
		}
	}
}

void AGameMode_Playable::OnMonsterSpawnCheck()
{
	auto& setCurLoadedLand = Managers::World->GetLoadedLandSet();

	// 현재 열려있는 랜드 레벨 기준으로 몬스터 on/off
	for(auto& tuple : m_mapMonsterSpawnPoint)
	{
		auto& strKey = tuple.Key;
		
		//Character Active
		if(setCurLoadedLand.Contains(strKey))
		{
			for(auto& pSpawnPoint : tuple.Value)
			{
				if(!IsValid(pSpawnPoint))
					continue;

				pSpawnPoint->SetActive_Character(true, m_bInit);
			}		
		}
		//Character Inactive
		else
		{
			for(auto& pSpawnPoint : tuple.Value)
			{
				if(!IsValid(pSpawnPoint))
					continue;

				pSpawnPoint->SetActive_Character(false);
			}
		}
	}
}

void AGameMode_Playable::OnInit_AfterLevelLoaded()
{
	// UI Open
	Managers::UI->OpenWindow<UWindow_GameMain>();

	Super::OnInit();
}

void AGameMode_Playable::OnStartGame()
{
	Super::OnStartGame();

	// Player active
	if(IsValid(m_pCharSpawnPoint))
		m_pCharSpawnPoint->SetActive_Character(true, false);

	// Monstser AI Start
	for(auto& tuple : m_mapMonsterSpawnPoint)
	{
		for(auto& pSpawnPoint : tuple.Value)
		{
			if(!IsValid(pSpawnPoint))
				continue;

			pSpawnPoint->OnStart_Character();
		}
	}
}

void AGameMode_Playable::OnEndGame()
{
	Super::OnEndGame();

	//필요한 일인가.. 고민
	if(IsValid(m_pCharSpawnPoint))
		m_pCharSpawnPoint->UnSpawnCharacter();

	for(auto& tuple : m_mapMonsterSpawnPoint)
	{
		for(auto& pSpawnPoint : tuple.Value)
		{
			pSpawnPoint->UnSpawnCharacter();
		}		
	}
}