#include "GameMode_InGame.h"
#include "Framework/Managers/Managers.h"
// #include "GameContents/SpawnSystem/SpawnSystem.h"

class UWindow_GameMain;

AGameMode_InGame::AGameMode_InGame()
	: AGameMode_Playable()
{
}

void AGameMode_InGame::OnInit()
{
	//InGame Level에서 바로 시작하는 경우 사용
	auto eWorldId = Managers::World->GetWorldID();
	if(eWorldId != EWorldId::InGame)
		Managers::World->InitLevelLandData(EWorldId::InGame);

	AGameMode_Playable::OnInit();
}
