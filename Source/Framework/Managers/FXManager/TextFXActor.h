// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FXActor.h"
#include "Components/TextRenderComponent.h"
#include "TextFXActor.generated.h"

/**
 * 
 */
UCLASS()
class FRAMEWORK_API ATextFXActor : public AFXActor
{
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* m_pTextComp;
	
	GENERATED_BODY()

public:
	ATextFXActor();
	void SetText(const FString& text);
};