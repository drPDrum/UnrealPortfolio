// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameCharacter.h"
#include "AnimInst.h"
#include "ANimController.generated.h"

/**
 * 
 */
UCLASS()
class FRAMEWORK_API UAnimController : public UObject
{
	GENERATED_BODY()
	
private:
	AGameCharacter* m_pOwner;
	UAnimInst* m_pAnimInstacne;

public:
	UAnimController();
	virtual ~UAnimController() override;

	void Init(AGameCharacter* owner);
	AGameCharacter* GetOwner();

	void SetAnimInst(TSubclassOf<UAnimInst> animInst);
	UAnimInst* GetAnimInst();

	void Play(UAnimMontage* animMontage, const float& fadeTime, const float& playRate, const bool& bResetIfPlaying, const EAnimBlendOption& blendOption);
	void Stop(const float& fOutTime, const EAnimBlendOption& blendOption);

	void OnOwnerTick(float deltaTime);
};