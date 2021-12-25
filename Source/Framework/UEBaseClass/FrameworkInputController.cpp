#include "FrameworkInputController.h"
#include "FrameworkCamera.h"
#include "Framework/Managers/Managers.h"
#include "GameFramework/InputSettings.h"

AFrameworkInputController::AFrameworkInputController(const FObjectInitializer& ObjectInitializer)
	: APlayerController(ObjectInitializer)
{
}

void AFrameworkInputController::SetCamera(AFrameworkCamera* cam)
{
	m_TargetGameCamera = cam;
	if (m_TargetGameCamera)
		SetViewTarget(m_TargetGameCamera);
}

void AFrameworkInputController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// Axis는 영구할당 한다. 삭제 메서드가 없음....
	InputComponent->BindAxis("AxisMoveX", this, &AFrameworkInputController::OnInput_AxisMoveX);
	InputComponent->BindAxis("AxisMoveY", this, &AFrameworkInputController::OnInput_AxisMoveY);
	InputComponent->BindAxis("AxisLookX", this, &AFrameworkInputController::OnInput_AxisLookX);
	InputComponent->BindAxis("AxisLookY", this, &AFrameworkInputController::OnInput_AxisLookY);
}

void AFrameworkInputController::ClearKey(const bool& bSave)
{
	auto pInput = UInputSettings::GetInputSettings();
	if (!pInput)
	{
		LOG_ERROR(TEXT("UInputSettings is NULL"));
		return;
	}

	// 기존 맵핑 제거
	auto pActionMappings = const_cast<TArray<FInputActionKeyMapping>*>(&pInput->GetActionMappings());
	pActionMappings->Empty(pActionMappings->Num());
	auto pAxisMappings = const_cast<TArray<FInputAxisKeyMapping>*>(&pInput->GetAxisMappings());
	pAxisMappings->Empty(pActionMappings->Num());

	// 기존 바인딩 제거
	InputComponent->ClearActionBindings();

	// ESC는 무조건 할당
	auto name_Escape = EKeys::Escape.GetFName();
	pInput->AddActionMapping(FInputActionKeyMapping(name_Escape, EKeys::Escape), false);
	InputComponent->BindAction(name_Escape, EInputEvent::IE_Released,  this, &AFrameworkInputController::OnInput_Escape);

	// 저장
	if (bSave)
	{
		pInput->ForceRebuildKeymaps();
		pInput->SaveKeyMappings();
	}
}

void AFrameworkInputController::SaveKey()
{
	auto pInput =  UInputSettings::GetInputSettings();
	if (!pInput)
	{
		LOG_ERROR(TEXT("UInputSettings is NULL"));
		return;
	}

	pInput->ForceRebuildKeymaps();
	pInput->SaveKeyMappings();
}

void AFrameworkInputController::AddKey_PlayCtrl(const EUserInputTypes& inputTypes, const bool& bReset = true, const bool& bSave = true)
{
	auto pInput =  UInputSettings::GetInputSettings();
	if (!pInput)
	{
		LOG_ERROR(TEXT("UInputSettings is NULL"));
		return;
	}

	if (bReset)
		ClearKey(false);

	// 타입별 세팅
	switch (inputTypes)
	{
	case EUserInputTypes::None:
		{
			// 저장하고 탈출
			if (bSave)
			{
				pInput->ForceRebuildKeymaps();
				pInput->SaveKeyMappings();
			}
		}
		return;
	case EUserInputTypes::KeyboardOnly:
		{
			// KeyboardOnly :: Axis
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisMoveX"), EKeys::Right, 1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisMoveX"), EKeys::Left, -1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisMoveY"), EKeys::Up, 1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisMoveY"), EKeys::Down, -1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisLookX"), EKeys::E, 1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisLookX"), EKeys::Q, -1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisLookY"), EKeys::T, 1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisLookY"), EKeys::G, -1), false);

			// KeyboardOnly :: Action
			pInput->AddActionMapping(FInputActionKeyMapping(FName("Attack00"), EKeys::D), false);
			pInput->AddActionMapping(FInputActionKeyMapping(FName("Attack01"), EKeys::F), false);
			pInput->AddActionMapping(FInputActionKeyMapping(FName("Jump"), EKeys::S), false);
			pInput->AddActionMapping(FInputActionKeyMapping(FName("Dash"), EKeys::A), false);
			pInput->AddActionMapping(FInputActionKeyMapping(FName("LookNearTarget"), EKeys::SpaceBar), false);
		}
		break;
	case EUserInputTypes::KeyboardAndMouse:
		{
			// KeyboardAndMouse :: Axis
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisMoveX"), EKeys::D, 1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisMoveX"), EKeys::A, -1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisMoveY"), EKeys::W, 1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisMoveY"), EKeys::S, -1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisLookX"), EKeys::MouseX, 1), false);
			pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisLookY"), EKeys::MouseY, 1), false);

			// KeyboardAndMouse :: Action
			pInput->AddActionMapping(FInputActionKeyMapping(FName("Attack00"), EKeys::LeftMouseButton), false);
			pInput->AddActionMapping(FInputActionKeyMapping(FName("Attack01"), EKeys::RightMouseButton), false);
			pInput->AddActionMapping(FInputActionKeyMapping(FName("Jump"), EKeys::LeftShift), false);
			pInput->AddActionMapping(FInputActionKeyMapping(FName("Dash"), EKeys::SpaceBar), false);
		}
		break;
	}

	// 조이스틱은 항상 포함
	
	// 조이스틱 :: Axis
	pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisMoveX"), EKeys::Gamepad_LeftX, 1), false);
	pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisMoveY"), EKeys::Gamepad_LeftY, 1), false);
	pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisLookX"), EKeys::Gamepad_RightX, 1), false);
	pInput->AddAxisMapping(FInputAxisKeyMapping(FName("AxisLookY"), EKeys::Gamepad_RightY, 1), false);

	// 조이스틱 :: Action
	pInput->AddActionMapping(FInputActionKeyMapping(FName("Attack00"), EKeys::Gamepad_FaceButton_Left), false);
	pInput->AddActionMapping(FInputActionKeyMapping(FName("Attack01"), EKeys::Gamepad_FaceButton_Top), false);
	pInput->AddActionMapping(FInputActionKeyMapping(FName("Jump"), EKeys::Gamepad_FaceButton_Right), false);
	pInput->AddActionMapping(FInputActionKeyMapping(FName("Dash"), EKeys::Gamepad_FaceButton_Bottom), false);
	pInput->AddActionMapping(FInputActionKeyMapping(FName("LookNearTarget"), EKeys::Gamepad_RightShoulder), false);

	// 키 바인딩
	InputComponent->BindAction("Attack00", EInputEvent::IE_Pressed,  this, &AFrameworkInputController::OnInput_KeyDownAttack00	);
	InputComponent->BindAction("Attack00", EInputEvent::IE_Released, this, &AFrameworkInputController::OnInput_KeyUpAttack00	);

	InputComponent->BindAction("Attack01", EInputEvent::IE_Pressed,  this, &AFrameworkInputController::OnInput_KeyDownAttack01	);
	InputComponent->BindAction("Attack01", EInputEvent::IE_Released, this, &AFrameworkInputController::OnInput_KeyUpAttack01	);

	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed,  this, &AFrameworkInputController::OnInput_KeyDownJump		);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &AFrameworkInputController::OnInput_KeyUpJump		);

	InputComponent->BindAction("Dash", EInputEvent::IE_Pressed,  this, &AFrameworkInputController::OnInput_KeyDownDash	);
	InputComponent->BindAction("Dash", EInputEvent::IE_Released, this, &AFrameworkInputController::OnInput_KeyUpDash );

	InputComponent->BindAction("LookNearTarget", EInputEvent::IE_Pressed, this, &AFrameworkInputController::OnInput_KeyDownLookNearTarget);
	InputComponent->BindAction("LookNearTarget", EInputEvent::IE_Released, this, &AFrameworkInputController::OnInput_KeyUpLookNearTarget);

	// 저장
	if (bSave)
	{
		pInput->ForceRebuildKeymaps();
		pInput->SaveKeyMappings();
	}
}

void AFrameworkInputController::SetMouseEnable(const bool& bEnable)
{
	if (bEnable)
		SetInputMode(FInputModeGameAndUI());
	else
		SetInputMode(FInputModeGameOnly());

	SetShowMouseCursor(bEnable);
}

template <class UserClass>
void AFrameworkInputController::AddKey(const FKey& key, UserClass* Object, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr Func, const bool& bSave = true)
{
	auto pInput =  UInputSettings::GetInputSettings();
	if (!pInput)
	{
		LOG_ERROR(TEXT("UInputSettings is NULL"));
		return;
	}

	auto name = key.GetFName();

	// 세팅 & 저장
	pInput->AddActionMapping(FInputActionKeyMapping(name, key), false);
	if (bSave)
	{
		pInput->ForceRebuildKeymaps();
		pInput->SaveKeyMappings();
	}

	// 키 바인딩
	InputComponent->BindAction(name, EInputEvent::IE_Pressed, Object, Func);
};
