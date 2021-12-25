// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventRuntimeBase.h"
#include "ActionEventData_FindTarget.h"
#include "ActionEventRuntime_FindTarget.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_FindTarget : public UObject, public ActionEventRuntimeBase<FActionEventData_FindTarget>
{
	GENERATED_BODY()
private:

	class UActionRuntime* m_pActionRuntime_ifNotFound;
	
public:
	UActionEventRuntime_FindTarget();
	virtual ~UActionEventRuntime_FindTarget() override;

	virtual void InitLinkedActionRuntime(const TMap<FString, UActionRuntime*>& map_ActionRuntimes) override;
	virtual bool OnStart(float elapsedTime) override;
};