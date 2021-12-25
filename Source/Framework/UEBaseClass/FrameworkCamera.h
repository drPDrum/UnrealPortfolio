#pragma once

#include "../CommonInfo.h"
#include "FrameworkCamera.generated.h"

UCLASS()
class FRAMEWORK_API AFrameworkCamera : public AActor
{
	GENERATED_BODY()

private:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* m_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* m_Body;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* m_Neck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* m_Head;

	// Target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* m_TargetActor;

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_fMinLength;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_fMaxLength;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_fYawDegreePerSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_fPitchDegreePerSec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector m_vOffsetToTarget;

	// Runtime
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_fYawDegree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float m_fPitchDegree;

	bool m_bNeedUpdateRotation;
	bool m_bUseZoomPerTargetVelocity;

public:	
	// Sets default values for this actor's properties
	AFrameworkCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetTarget(AActor* targetActor);
	virtual UCameraComponent* GetCamera();

	void OnInput_AxisLookX(float scale);
	void OnInput_AxisLookY(float scale);
};