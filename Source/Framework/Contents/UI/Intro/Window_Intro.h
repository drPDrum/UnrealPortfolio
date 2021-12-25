// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/Managers/UIManager/UIBase/WindowBase.h"
#include "Window_Intro.generated.h"

class UButton;

UCLASS()
class FRAMEWORK_API UWindow_Intro : public UWindowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window Intro", meta=(BindWidget, AllowPrivateAccess="true", DisplayName="Btn Start"))
	UButton* BtnStart;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window Intro", meta=(BindWidget, AllowPrivateAccess="true", DisplayName="Canvas CenterRoot"))
	UCanvasPanel* Canvas_CenterRoot;

public:
	UWindow_Intro(const FObjectInitializer& ObjectInitializer);
	virtual void OnWindowCreate() override;

protected:
	virtual void InitHotkey() override;

private:
	UFUNCTION()
	void OnClick_StartGame();
	UFUNCTION(BlueprintCallable)
	void OnClick_UIModule_Help();
};