#pragma once

#include "../CommonInfo.h"
#include "FrameworkGameModeBase.generated.h"

class CSpawnData;
UCLASS()
class FRAMEWORK_API AFrameworkGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFrameworkGameModeBase();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	// 상속 후 리턴 직전에 꼭 호출해야함
	virtual void OnInit();
	virtual void OnStartGame();
	virtual void OnEndGame();
};