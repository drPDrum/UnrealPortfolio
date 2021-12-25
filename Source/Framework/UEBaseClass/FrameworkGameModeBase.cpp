#include "FrameworkGameModeBase.h"

#include "Framework/Managers/Managers.h"

AFrameworkGameModeBase::AFrameworkGameModeBase()
{
	// bUseSeamlessTravel = true;	
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;
	// 상속 받은 곳에서 필요한 InputController 세팅
	PlayerControllerClass = AFrameworkInputController::StaticClass();
	GameStateClass = nullptr;
	SpectatorClass = nullptr;

	// GameMode Tick이 필요할 때 주석 해제
	//PrimaryActorTick.bCanEverTick = true;
}

void AFrameworkGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (!Managers::IsInit())
		Managers::Init(GetWorld());

	// 로드를 시작할 것임을 알림
	Managers::World->OnStart_GameModeInit(this);
	OnInit();
}

void AFrameworkGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AFrameworkGameModeBase::OnInit()
{
	// 로드가 끝났음을 알림
	Managers::World->OnEnd_GameModeInit();
}

void AFrameworkGameModeBase::OnStartGame()
{
	// 컨트롤러 작동 가능하도록 변경, 만약 딜레이가 필요하다면 상속 후 적당한 타이밍에 열어줄 것
	Managers::UI->GetInputCtrl()->SetActorTickEnabled(true);
}

void AFrameworkGameModeBase::OnEndGame()
{
	
}