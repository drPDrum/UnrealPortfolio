// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_Game.h"
#include "Framework/Managers/Managers.h"
// #include "GameContents/SpawnSystem/SpawnSystem.h"

class UWindow_GameMain;

AGameMode_Game::AGameMode_Game()
	: AGameMode_Playable()
{
}

void AGameMode_Game::OnInit()
{
	//InGame Level에서 바로 시작하는 경우 사용
	auto eWorldId = Managers::World->GetWorldID();
	if(eWorldId != EWorldId::Game)
		Managers::World->InitLevelLandData(EWorldId::Game);

	AGameMode_Playable::OnInit();
}
