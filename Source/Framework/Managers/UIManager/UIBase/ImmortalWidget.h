// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "Blueprint/UserWidget.h"
#include "ImmortalWidget.generated.h"

UCLASS()
class FRAMEWORK_API UImmortalWidget : public UUserWidget
{
	GENERATED_BODY()	
private:
	bool m_bInitWidgetAnim;
	
	UPROPERTY()
	TMap<FString, UWidgetAnimation*> m_mapWidgetAnim;

public:
	UImmortalWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	UWidgetAnimation* GetAnimation(const FString& animName);
};
