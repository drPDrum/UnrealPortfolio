// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDataBase.h"
#include "ActionEventData_PlayFX.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_PlayFX : public FActionEventDataBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FString m_ResPath_FX;

	UPROPERTY(EditAnywhere)
	FName m_SocketName;
	
	UPROPERTY(EditAnywhere)
	FVector m_vLocation;
	
	UPROPERTY(EditAnywhere)
	FQuat m_qRotation;

	UPROPERTY(EditAnywhere)
	FVector m_vScale;

	UPROPERTY(EditAnywhere)
	bool m_bIsFollow;

public:
	FActionEventData_PlayFX();
	virtual ~FActionEventData_PlayFX() override;
	
	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;
	virtual ActionEventTypes GetActionEventType() const override;
};
