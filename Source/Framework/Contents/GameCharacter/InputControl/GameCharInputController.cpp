#include "GameCharInputController.h"

#include "Framework/Contents/GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "Framework/Managers/Managers.h"
#include "Framework/UEBaseClass/FrameworkCamera.h"

AGameCharInputController::AGameCharInputController(const FObjectInitializer& ObjectInitializer)
	: AFrameworkInputController(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	m_fCheckLandRemainTime = 0.5f;
}

AGameCharacter* AGameCharInputController::GetPlayerCharacter()
{
	return m_TargetCharacter;
}

void AGameCharInputController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	m_TargetCharacter = Cast<AGameCharacter>(InPawn);
	
	if (m_TargetCharacter && IsValid(m_TargetCharacter))
	{
		m_TargetCharacter->GetBrainCtrl()->SetOrderByUserInput(true);
		m_TargetCharacter->GetBrainCtrl()->StopAI();
		m_TargetCharacter->RefreshCollisionProfile();
		AGameCharacter::AllyInfo->SetPlayerTeamInfo(m_TargetCharacter->GetTeamInfo());
	}
}

void AGameCharInputController::OnUnPossess()
{
	Super::OnUnPossess();

	if (m_TargetCharacter && IsValid(m_TargetCharacter))
	{
		// AI로 다시 빙의시킨다.
		m_TargetCharacter->GetBrainCtrl()->Possess(m_TargetCharacter);
		
		m_TargetCharacter->GetBrainCtrl()->SetOrderByUserInput(false);
		m_TargetCharacter->GetBrainCtrl()->StartAI();
		m_TargetCharacter->RefreshCollisionProfile();
		AGameCharacter::AllyInfo->SetPlayerTeamInfo(nullptr);
	}

	m_TargetCharacter = nullptr;
}

void AGameCharInputController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Managers::World -> 현재 어느 위치에 있는지 체크
	m_fCheckLandRemainTime -= DeltaSeconds;
	if (Managers::IsInit() && m_TargetCharacter && m_fCheckLandRemainTime <= 0.f)
	{
		m_fCheckLandRemainTime = 0.5f;
		Managers::World->CheckAndLoadLand(m_TargetCharacter);
	}
}

void AGameCharInputController::OnInput_AxisMoveX(float fScale)
{
	if (m_TargetCharacter)
		m_TargetCharacter->GetBrainCtrl()->OnInput_MoveAxisX(fScale);
}

void AGameCharInputController::OnInput_AxisMoveY(float fScale)
{
	if (m_TargetCharacter)
		m_TargetCharacter->GetBrainCtrl()->OnInput_MoveAxisY(fScale);
}

void AGameCharInputController::OnInput_AxisLookX(float fScale)
{
	if (m_TargetGameCamera)
	{
		m_TargetGameCamera->OnInput_AxisLookX(fScale);
		if (m_TargetCharacter)
			m_TargetCharacter->GetBrainCtrl()->SetLookInfo(m_TargetGameCamera->GetActorRotation().Quaternion());
	}
}

void AGameCharInputController::OnInput_AxisLookY(float fScale)
{
	if (m_TargetGameCamera)
	{
		m_TargetGameCamera->OnInput_AxisLookY(fScale);
	}
}

void AGameCharInputController::OnInput_KeyDownAttack00()
{
	if (m_TargetCharacter)
		m_TargetCharacter->GetBrainCtrl()->OnInput_Key(ETriggerEventTypes::Attack00_Down);
}

void AGameCharInputController::OnInput_KeyUpAttack00()
{
	if (m_TargetCharacter)
		m_TargetCharacter->GetBrainCtrl()->OnInput_Key(ETriggerEventTypes::Attack00_Up);
}

void AGameCharInputController::OnInput_KeyDownAttack01()
{
	if (m_TargetCharacter)
		m_TargetCharacter->GetBrainCtrl()->OnInput_Key(ETriggerEventTypes::Attack01_Down);
}

void AGameCharInputController::OnInput_KeyUpAttack01()
{
	if (m_TargetCharacter)
		m_TargetCharacter->GetBrainCtrl()->OnInput_Key(ETriggerEventTypes::Attack01_Up);
}

void AGameCharInputController::OnInput_KeyDownJump()
{
	if (m_TargetCharacter)
		m_TargetCharacter->GetBrainCtrl()->OnInput_Key(ETriggerEventTypes::Jump_Down);
}

void AGameCharInputController::OnInput_KeyUpJump()
{
	if (m_TargetCharacter)
		m_TargetCharacter->GetBrainCtrl()->OnInput_Key(ETriggerEventTypes::Jump_Up);
}

void AGameCharInputController::OnInput_KeyDownDash()
{
	if (m_TargetCharacter)
		m_TargetCharacter->GetBrainCtrl()->OnInput_Key(ETriggerEventTypes::Dash_Down);
}

void AGameCharInputController::OnInput_KeyUpDash()
{
	if (m_TargetCharacter)
		m_TargetCharacter->GetBrainCtrl()->OnInput_Key(ETriggerEventTypes::Dash_Up);
}

void AGameCharInputController::OnInput_KeyDownLookNearTarget()
{
	if (m_TargetCharacter && m_TargetGameCamera)
	{
		// Todo
	}
}

void AGameCharInputController::OnInput_KeyUpLookNearTarget()
{
	if (m_TargetCharacter && m_TargetGameCamera)
	{
		// Todo
	}
}

void AGameCharInputController::OnInput_Escape()
{
	// TODO
}
