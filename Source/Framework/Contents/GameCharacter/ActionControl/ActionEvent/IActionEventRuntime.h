// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "RuntimeUtils/GenericEx/DoubleLinkedListEx.h"


class FRAMEWORK_API IActionEventRuntime
{
public:
	
	IActionEventRuntime();
	virtual ~IActionEventRuntime();

	virtual bool IsDuration() = 0;

	virtual float GetStartTime() = 0;
	virtual float GetEndTime() = 0;
	virtual bool GetEndOnActionEnd() = 0;

	virtual bool OnStart(float elapsedTime) = 0;
	virtual bool OnTick(float elapsedTime, float deltaTime) = 0;
	virtual void OnEnd(bool bIsForLoopReset) = 0;

	virtual void InitLinkedActionRuntime(const TMap<FString, class UActionRuntime*>& map_ActionRuntimes) = 0;

	virtual TDoubleLinkedListEx<IActionEventRuntime*>::TDoubleLinkedListExNode* GetNode() = 0;
};

struct FRAMEWORK_API CreateRuntimeResult
{
public:
	UObject* pUObject;
	IActionEventRuntime* pRuntime;
};