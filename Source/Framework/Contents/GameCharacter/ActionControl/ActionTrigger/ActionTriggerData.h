// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameCharacter_Define.h"
#include "ActionTriggerData.generated.h"

UENUM()
enum class ETriggerAreaTypes : uint8
{
	Any,
	Ground,
	Air,
	Water,
	GroundAndAir,
	GroundAndWater,
	AirAndWater
	
};

USTRUCT()
struct FRAMEWORK_API FActionTriggerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ETriggerAreaTypes AreaType;

	UPROPERTY(EditAnywhere)
	FString NextActionName;

	UPROPERTY(EditAnywhere)
	float fStartTime;
	UPROPERTY(EditAnywhere)
	bool bSameAsActionEnd_StartTime;
	UPROPERTY(EditAnywhere)
	float fEndTime;
	UPROPERTY(EditAnywhere)
	bool bSameAsActionEnd_EndTime;

	UPROPERTY(EditAnywhere)
	ETriggerAxisTypes AxisType;
	
	UPROPERTY(EditAnywhere)
	ETriggerEventTypes EventType;

public:
	FActionTriggerData();
	~FActionTriggerData();
};