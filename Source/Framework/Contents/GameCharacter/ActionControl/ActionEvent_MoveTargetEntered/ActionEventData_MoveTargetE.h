// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationDataBase.h"
#include "ActionEventData_MoveTargetE.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_MoveTargetE : public FActionEventDurationDataBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float m_fMoveSpdPerSec;

	UPROPERTY(EditAnywhere)
	FString OnReachTargetPlay;
	UPROPERTY(EditAnywhere)
	float m_fOnReachDist;
	
public:
	FActionEventData_MoveTargetE();
	virtual ~FActionEventData_MoveTargetE() override;

	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;
	virtual ActionEventTypes GetActionEventType() const override;
};