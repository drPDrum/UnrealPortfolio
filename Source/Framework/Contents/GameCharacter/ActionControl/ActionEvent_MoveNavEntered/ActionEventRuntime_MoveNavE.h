// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_MoveNavE.h"
#include "ActionEventRuntime_MoveNavE.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_MoveNavE : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_MoveNavE>
{
	GENERATED_BODY()

private:
	bool m_bIsValidTarget;
	FVector m_vDstWorldPos;
	
public:
	UActionEventRuntime_MoveNavE();
	virtual ~UActionEventRuntime_MoveNavE() override;
	
	virtual bool OnStart(float elapsedTime) override;
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
	virtual void OnEnd(bool bIsForLoopReset) override;
};