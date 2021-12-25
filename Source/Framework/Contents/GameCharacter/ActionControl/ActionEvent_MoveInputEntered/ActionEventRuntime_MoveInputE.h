// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_MoveInputE.h"
#include "ActionEventRuntime_MoveInputE.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_MoveInputE : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_MoveInputE>
{
	GENERATED_BODY()

private:
	FVector m_vDstWorldDir;
	
public:
	UActionEventRuntime_MoveInputE();
	virtual ~UActionEventRuntime_MoveInputE() override;
	
	virtual bool OnStart(float elapsedTime) override;
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
};
