// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ActionEvent/ActionEventDataBase.h"
#include "Framework/Contents/GameCharacter/GameCharacter_Define.h"
#include "ActionEventData_HitShape.generated.h"

UENUM()
enum class EDirTypes : uint8
{
	MyForward,
	Input,	
};

UENUM()
enum class ECastTypes : uint8
{
	MyPos_Sphere,
	MyPos_Box,
	Target_Sphere,
	Target_Box,
};

USTRUCT()
struct FRAMEWORK_API FHitShapeInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	ECastTypes m_CastType;

	UPROPERTY(EditAnywhere)
	FVector m_vOffsetPos;
	UPROPERTY(EditAnywhere)
	FQuat m_qOffsetRot;
	UPROPERTY(EditAnywhere)
	FVector m_Size;

	UPROPERTY(EditAnywhere)
	float m_fDist;

	FHitShapeInfo();
	~FHitShapeInfo();
};

USTRUCT()
struct FRAMEWORK_API FActionEventData_HitShape : public FActionEventDataBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	EGameCharacterFindTargetTypes m_TargetType;
	UPROPERTY(EditAnywhere)
	TArray<FHitShapeInfo> m_hitInfos;

	UPROPERTY(EditAnywhere)
	FString m_ResPath_SFX;
	UPROPERTY(EditAnywhere)
	FString m_ResPath_FX;

	UPROPERTY(EditAnywhere)
	bool m_bIsFollow;

public:
	FActionEventData_HitShape();
	virtual ~FActionEventData_HitShape() override;
	
	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;
	virtual ActionEventTypes GetActionEventType() const override;
};
