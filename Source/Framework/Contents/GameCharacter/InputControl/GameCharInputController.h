#pragma once

#include "Framework/CommonInfo.h"
#include "Framework/UEBaseClass/FrameworkInputController.h"
#include "GameCharInputController.generated.h"

class AGameCamera;
class AGameCharacter;

UCLASS()
class FRAMEWORK_API AGameCharInputController : public AFrameworkInputController
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	AGameCharacter* m_TargetCharacter;

	float m_fCheckLandRemainTime;

public:	
	AGameCharInputController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	AGameCharacter* GetPlayerCharacter();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
	void OnInput_AxisMoveX(float fScale);
	void OnInput_AxisMoveY(float fScale);
	void OnInput_AxisLookX(float fScale);
	void OnInput_AxisLookY(float fScale);

	void OnInput_KeyDownAttack00();
	void OnInput_KeyUpAttack00();

	void OnInput_KeyDownAttack01();
	void OnInput_KeyUpAttack01();

	void OnInput_KeyDownJump();
	void OnInput_KeyUpJump();

	void OnInput_KeyDownDash();
	void OnInput_KeyUpDash();
	
	void OnInput_KeyDownLookNearTarget();
	void OnInput_KeyUpLookNearTarget();

	void OnInput_Escape();
};
