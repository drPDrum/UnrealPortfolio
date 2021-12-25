// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "Animation/AnimInstance.h"
#include "AnimInst.generated.h"

UENUM(BlueprintType)
enum class EAnimBlendOption : uint8
{
	None,
	BlendOption00,
	BlendOption01,
	BlendOption02,
};

UCLASS()
class FRAMEWORK_API UAnimInst : public UAnimInstance
{
	class AGameCharacter* m_pOwner;
	
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	float m_BlendTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	EAnimBlendOption m_BlendOption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	bool m_MontageIsPlaying;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	float m_fAngleBetweenForwardAndMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"));
	float m_fMoveSize;

public:
	void Init(AGameCharacter* pOwner);
	void OnTick(float deltaTime);
	void Play(UAnimMontage* animMontage, const float& fadeTime, const float& playRate, const bool& bResetIfPlaying, const EAnimBlendOption& blendOption);
	void Stop(const float& fOutTime, const EAnimBlendOption& blendOption);
};