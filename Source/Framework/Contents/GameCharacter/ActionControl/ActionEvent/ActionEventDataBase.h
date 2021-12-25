// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IActionEventRuntime.h"
#include "ActionEventDataBase.generated.h"

class AGameCharacter;

enum class ActionEventTypes
{
	None,
	FindTarget,
	
	MoveByInputEntered,
	MoveByInputUpdated,
	MoveToTargetEntered,
	MoveToTargetUpdated,
	MoveNavEntered,
	MoveNavUpdated,

	Rotate,
	RotateByInputEntered,
	RotateByInputUpdated,
	RotateToTargetEntered,
	RotateToTargetUpdated,
	RotateToVelocity,

	Animation,
	PlaySFX,
	PlayFX,

	HitShape,
};

USTRUCT()
struct FRAMEWORK_API FActionEventDataBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float fStartTime;

public:
	FActionEventDataBase();
	virtual ~FActionEventDataBase();

	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner);

	virtual ActionEventTypes GetActionEventType() const;

	virtual bool IsDuration() const;
};