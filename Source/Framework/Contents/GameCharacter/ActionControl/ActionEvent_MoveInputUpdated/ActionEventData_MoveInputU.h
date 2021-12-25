// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationDataBase.h"
#include "ActionEventData_MoveInputU.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_MoveInputU : public FActionEventDurationDataBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float m_fMoveSpdPerSec;
	
	UPROPERTY(EditAnywhere)
	bool m_bUseInputScale;
	
	UPROPERTY(EditAnywhere)
	bool m_bUseUpdateAngleSpd;

	UPROPERTY(EditAnywhere)
	float m_fUpdateAngleSpd;

public:
	FActionEventData_MoveInputU();
	virtual ~FActionEventData_MoveInputU() override;

	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;
	virtual ActionEventTypes GetActionEventType() const override;
};
