// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionEventDataBase.h"
#include "ActionEventDurationDataBase.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventDurationDataBase : public FActionEventDataBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float fEndTime;
	UPROPERTY(EditAnywhere)
	bool bSameAsActionEnd;

public:
	FActionEventDurationDataBase();
	virtual ~FActionEventDurationDataBase() override;

	virtual bool IsDuration() const override;
};