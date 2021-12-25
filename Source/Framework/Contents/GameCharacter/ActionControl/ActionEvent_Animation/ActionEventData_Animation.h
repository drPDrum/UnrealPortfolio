// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDataBase.h"
#include "../../AnimControl/AnimInst.h"
#include "ActionEventData_Animation.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_Animation : public FActionEventDataBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString m_AnimName;
	UPROPERTY(EditAnywhere)
	float m_fFadeTime;
	UPROPERTY(EditAnywhere)
	float m_fPlayRate;
	UPROPERTY(EditAnywhere)
	bool m_bResetIfPlaying;
	UPROPERTY(EditAnywhere)
	EAnimBlendOption m_BlendOption;

public:
	FActionEventData_Animation();
	virtual ~FActionEventData_Animation() override;

	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;
	virtual ActionEventTypes GetActionEventType() const override;
};