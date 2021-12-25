// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventRuntimeBase.h"
#include "ActionEventData_Animation.h"
#include "ActionEventRuntime_Animation.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_Animation : public UObject, public ActionEventRuntimeBase<FActionEventData_Animation>
{
	GENERATED_BODY()
private:

	UAnimMontage* m_AnimMontage;

public:
	UActionEventRuntime_Animation();
	virtual ~UActionEventRuntime_Animation() override;

	virtual void Init(FActionEventData_Animation& data, AGameCharacter* owner) override;
	virtual bool OnStart(float elapsedTime) override;
};