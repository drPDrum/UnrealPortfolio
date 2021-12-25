// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionTriggerData.h"

FActionTriggerData::FActionTriggerData()
{
	AreaType = ETriggerAreaTypes::Ground;
	AxisType = ETriggerAxisTypes::None;
	EventType = ETriggerEventTypes::None;

	fStartTime = 0.f;
	fEndTime = 99999.f;
	bSameAsActionEnd_StartTime = false;
	bSameAsActionEnd_EndTime = false;
}

FActionTriggerData::~FActionTriggerData()
{
}