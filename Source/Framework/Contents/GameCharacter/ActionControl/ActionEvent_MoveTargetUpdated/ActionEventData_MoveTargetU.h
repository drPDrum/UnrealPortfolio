// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationDataBase.h"
#include "ActionEventData_MoveTargetU.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_MoveTargetU : public FActionEventDurationDataBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float m_fMoveSpdPerSec;

	UPROPERTY(EditAnywhere)
	bool m_bUseUpdateAngleSpd;

	UPROPERTY(EditAnywhere)
	float m_fUpdateAngleSpd;
	
	UPROPERTY(EditAnywhere)
	FString OnReachTargetPlay;
	UPROPERTY(EditAnywhere)
	float m_fOnReachDist;
	
public:
	FActionEventData_MoveTargetU();
	virtual ~FActionEventData_MoveTargetU() override;

	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;
	virtual ActionEventTypes GetActionEventType() const override;
};