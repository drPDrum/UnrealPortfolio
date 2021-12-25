#pragma once
#include "Gamemode_Playable.h"
#include "GameMode_InGame.generated.h"


UCLASS()
class FRAMEWORK_API AGameMode_InGame : public AGameMode_Playable
{
	GENERATED_BODY()

public:
	AGameMode_InGame();

	virtual void OnInit() override;
};