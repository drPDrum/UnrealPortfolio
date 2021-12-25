// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Framework/UEBaseClass/FrameworkGameModeBase.h"
#include "GameMode_Intro.generated.h"

/**
* 
*/
UCLASS()
class FRAMEWORK_API AGameMode_Intro : public AFrameworkGameModeBase
{
	GENERATED_BODY()

public:
	AGameMode_Intro();

	virtual void OnInit() override;
};