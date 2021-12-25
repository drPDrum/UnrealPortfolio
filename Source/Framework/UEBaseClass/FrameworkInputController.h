// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../CommonInfo.h"
#include "GameFramework/PlayerController.h"
#include "FrameworkInputController.generated.h"

class AFrameworkCamera;
class AGameCharacter;

UENUM()
enum class EUserInputTypes : uint8
{
	None,
	KeyboardOnly,
	KeyboardAndMouse,
};

UCLASS()
class FRAMEWORK_API AFrameworkInputController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	AFrameworkCamera* m_TargetGameCamera;

public:
	AFrameworkInputController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void SetCamera(AFrameworkCamera* cam);

protected:
	virtual void SetupInputComponent() override;
	// TODO : 상속받는 곳에서 빙의, 해제 관련 로직 만들어 줄 것.
	// virtual void OnPossess(APawn* InPawn) override;
	// virtual void OnUnPossess() override;

	// 입력 처리. 상속받아 필요 입력처리를 진행한다.
public:	
	virtual void OnInput_AxisMoveX(float scale) { }
	virtual void OnInput_AxisMoveY(float scale) { }
	virtual void OnInput_AxisLookX(float scale) { }
	virtual void OnInput_AxisLookY(float scale) { }

	virtual void OnInput_KeyDownAttack00() { }
	virtual void OnInput_KeyUpAttack00() { }
	virtual void OnInput_KeyDownAttack01() { }
	virtual void OnInput_KeyUpAttack01() { }
	virtual void OnInput_KeyDownJump() { }
	virtual void OnInput_KeyUpJump() { }
	virtual void OnInput_KeyDownDash() { }
	virtual void OnInput_KeyUpDash() { }	
	virtual void OnInput_KeyDownLookNearTarget() { }
	virtual void OnInput_KeyUpLookNearTarget() { }
	virtual void OnInput_Escape() { }

public:
	void ClearKey(const bool& bSave);
	void SaveKey();
	void AddKey_PlayCtrl(const EUserInputTypes& inputTypes, const bool& bReset,  const bool& bSave);

	template<class UserClass>
	void AddKey(const FKey& key, UserClass* Object, typename FInputActionHandlerSignature::TUObjectMethodDelegate< UserClass >::FMethodPtr Func, const bool& bSave);
	void SetMouseEnable(const bool& bEnable);
};
