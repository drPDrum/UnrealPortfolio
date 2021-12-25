// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationDataBase.h"
#include "ActionEventData_MoveNavE.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_MoveNavE : public FActionEventDurationDataBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	float m_fMoveSpdPerSec;
	UPROPERTY(EditAnywhere)
	bool m_bRoateToMoveDir;
	
public:
	FActionEventData_MoveNavE();
	virtual ~FActionEventData_MoveNavE() override;

	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;
	virtual ActionEventTypes GetActionEventType() const override;
};