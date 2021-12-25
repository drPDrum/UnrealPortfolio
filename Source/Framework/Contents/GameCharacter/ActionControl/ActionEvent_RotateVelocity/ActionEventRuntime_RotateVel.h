// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_RotateVel.h"
#include "ActionEventRuntime_RotateVel.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_RotateVel : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_RotateVel>
{
	GENERATED_BODY()
public:
	UActionEventRuntime_RotateVel();
	virtual ~UActionEventRuntime_RotateVel() override;

	virtual bool OnTick(float elapsedTime, float deltaTime) override;
};
