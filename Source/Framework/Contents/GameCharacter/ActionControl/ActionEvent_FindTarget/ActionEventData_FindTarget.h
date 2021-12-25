// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDataBase.h"
#include "Framework/Contents/GameCharacter/GameCharacter_Define.h"
#include "ActionEventData_FindTarget.generated.h"



USTRUCT()
struct FRAMEWORK_API FActionEventData_FindTarget : public FActionEventDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float m_fRadius;

	UPROPERTY(EditAnywhere)
	EGameCharacterFindTargetTypes m_TargetType;

	UPROPERTY(EditAnywhere)
	EGameCharacterStateTypes m_TargetStateType;

	UPROPERTY(EditAnywhere)
	FString m_ifNotFoundPlay;
	
public:
	FActionEventData_FindTarget();
	virtual ~FActionEventData_FindTarget() override;

	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;
	virtual ActionEventTypes GetActionEventType() const override;
};
