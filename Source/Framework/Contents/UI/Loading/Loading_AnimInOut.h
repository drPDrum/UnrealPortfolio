// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Framework/Managers/UIManager/UIBase/LoadingBase.h"

#include "Loading_AnimInOut.generated.h"

class UWidgetAnimation;

UCLASS()
class FRAMEWORK_API ULoading_AnimInOut : public ULoadingBase
{
	GENERATED_BODY()

public:
	virtual void Open() override;
	virtual void Close() override;
};