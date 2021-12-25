// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationDataBase.h"
#include "ActionEventData_MoveNavU.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_MoveNavU : public FActionEventDurationDataBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float m_fMoveSpdPerSec;
	UPROPERTY(EditAnywhere)
	bool m_bRoateToMoveDir;

public:
	FActionEventData_MoveNavU();
	virtual ~FActionEventData_MoveNavU() override;

	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;
	virtual ActionEventTypes GetActionEventType() const override;
};