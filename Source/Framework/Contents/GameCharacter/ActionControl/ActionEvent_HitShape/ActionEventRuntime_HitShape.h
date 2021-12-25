// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventRuntimeBase.h"
#include "ActionEventData_HitShape.h"
#include "Framework/Managers/FXManager/FXManager.h"
#include "ActionEventRuntime_HitShape.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_HitShape :  public UObject, public ActionEventRuntimeBase<FActionEventData_HitShape>
{
	GENERATED_BODY()

	TArray<FCollisionShape> m_Shapes;
	
	FXContainer* m_pFXContainer;
	USoundBase* m_pSoundBase;

public:
	UActionEventRuntime_HitShape();
	virtual ~UActionEventRuntime_HitShape() override;

	virtual void Init(FActionEventData_HitShape& data, AGameCharacter* owner) override;
	virtual bool OnStart(float elapsedTime) override;
};