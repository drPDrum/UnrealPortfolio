// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Gamemode_Playable.h"
#include "GameMode_Game.generated.h"

UCLASS()
class FRAMEWORK_API AGameMode_Game : public AGameMode_Playable
{
	GENERATED_BODY()

public:
	AGameMode_Game();

	virtual void OnInit() override;
};