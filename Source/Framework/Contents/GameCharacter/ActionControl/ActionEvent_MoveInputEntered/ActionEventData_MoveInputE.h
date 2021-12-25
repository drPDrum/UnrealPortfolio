// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationDataBase.h"
#include "ActionEventData_MoveInputE.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_MoveInputE : public FActionEventDurationDataBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float m_fMoveSpdPerSec;
	
public:
	FActionEventData_MoveInputE();
	virtual ~FActionEventData_MoveInputE() override;

	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;
	virtual ActionEventTypes GetActionEventType() const override;
};
