// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_MoveTargetE.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "ActionEventRuntime_MoveTargetE.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_MoveTargetE : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_MoveTargetE>
{
	GENERATED_BODY()

	FVector m_vDstWorldDir;
	FVector m_vDstWorldPos;
	GameCharacterTargetInfo m_TargetInfo;

	class UActionRuntime* m_pActionRuntime_OnReachTarget;
	
public:
	UActionEventRuntime_MoveTargetE();
	virtual ~UActionEventRuntime_MoveTargetE() override;
	
	virtual void InitLinkedActionRuntime(const TMap<FString, UActionRuntime*>& map_ActionRuntimes) override;
	
	virtual bool OnStart(float elapsedTime) override;
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
	
	bool CheckSweep() const;
	bool CheckDistance() const;
};
