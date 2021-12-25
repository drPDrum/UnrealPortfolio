#pragma once

#include "../../CommonInfo.h"
#include "Framework/UEBaseClass/FrameworkGameModeBase.h"
#include "GameMode_ManagerTest.generated.h"

UCLASS()
class FRAMEWORK_API AGameMode_ManagerTest : public AFrameworkGameModeBase
{
	GENERATED_BODY()
public:	
	AGameMode_ManagerTest();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 상속 후 리턴 직전에 꼭 호출해야함
	virtual void OnInit();
	virtual void OnStartGame();
	virtual void OnEndGame();
	
	private:
	void PrintFPaths();
	void TestCode();

	void TestCodeManagers();
	void TestCodeDataManager();
	void TestCodeLocalizationManager();
	void TestCodeUIManager();
	void TestCodePopup(int32 nTypeID, bool bUseBasicButton = true);

	void OnOkCallback(int32 nValue);
	void OnCancelCallback(int32 nValue);
	void OnNoCallback(int32 nValue);

	//GameMode_ManagerTest+Input.cpp
	void OnTickDebugInputHandler();
};
