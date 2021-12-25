// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_MoveTargetU.h"
#include "../../BrainControl/BrainController.h"
#include "ActionEventRuntime_MoveTargetU.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_MoveTargetU : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_MoveTargetU>
{
	GENERATED_BODY()

	FVector m_vDstWorldDir;
	GameCharacterTargetInfo m_TargetInfo;
	
	class UActionRuntime* m_pActionRuntime_OnReachTarget;
	
public:
	UActionEventRuntime_MoveTargetU();
	virtual ~UActionEventRuntime_MoveTargetU() override;
	
	virtual void InitLinkedActionRuntime(const TMap<FString, UActionRuntime*>& map_ActionRuntimes) override;
	
	virtual bool OnStart(float elapsedTime) override;
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
	bool CheckSweep() const;
	bool CheckDistance() const;
};
