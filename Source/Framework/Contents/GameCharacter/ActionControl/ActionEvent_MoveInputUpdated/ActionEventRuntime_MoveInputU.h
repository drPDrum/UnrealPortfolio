// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_MoveInputU.h"
#include "ActionEventRuntime_MoveInputU.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_MoveInputU : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_MoveInputU>
{
	GENERATED_BODY()

	FVector m_vDstWorldDir;
	
public:
	UActionEventRuntime_MoveInputU();
	virtual ~UActionEventRuntime_MoveInputU() override;
	
	virtual bool OnStart(float elapsedTime) override;
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
};
