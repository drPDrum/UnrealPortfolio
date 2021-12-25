// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_MoveNavU.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "ActionEventRuntime_MoveNavU.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_MoveNavU : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_MoveNavU>
{
	GENERATED_BODY()

private:

	GameCharacterTargetInfo m_TargetInfo;
	bool m_bIsActiveNav;
	
public:
	UActionEventRuntime_MoveNavU();
	virtual ~UActionEventRuntime_MoveNavU() override;
	
	virtual bool OnStart(float elapsedTime) override;
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
	virtual void OnEnd(bool bIsForLoopReset) override;
};