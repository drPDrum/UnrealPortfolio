// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../CommonInfo.h"
#include "UObject/Object.h"
#include "SurveyInfo.generated.h"

class AGameCharacter;

UENUM()
enum class ESurveyTargetTypes : uint8
{
	Friendlay_Nearest,
	Friendlay_Farthest,
	Friendly_HP_Highest,
	Friendly_HP_Lowest,
	Enemy_Nearest,
	Enemy_Farthest,
	Enemy_HP_Highest,
	Enemy_HP_Lowest,
};

struct SurveyGameCharacterInfo
{
	float fValue;
	AGameCharacter* pTarget;
	FBodyInstance* pBody;

	void RefreshMinInfo(const float& fNewValue, AGameCharacter* const pNewTarget, FBodyInstance* const pNewBody);
	void RefreshMaxInfo(const float& fNewValue, AGameCharacter* const pNewTarget, FBodyInstance* const pNewBody);

	void Clear(float fClearValue);
};

UCLASS()
class FRAMEWORK_API USurveyInfo : public UObject
{
	GENERATED_BODY()

public:
	int nCount_Friendly_Alive;
	int nCount_Friendly_Die;
	int nCount_Enemy_Alive;
	int nCount_Enemy_Die;

	// 가장 가까운 아군
	SurveyGameCharacterInfo Friendly_Nearest;
	// 가장 먼 아군
	SurveyGameCharacterInfo Friendly_Farthest;
	// 가장 HP가 많은 아군
	SurveyGameCharacterInfo Friendly_HPRateHighest;
	// 가장 HP가 적은 아군
	SurveyGameCharacterInfo Friendly_HPRateNearest;

	// 가장 가까운 적군
	SurveyGameCharacterInfo Enemy_Nearest;
	// 가장 먼 적군
	SurveyGameCharacterInfo Enemy_Farthest;
	// 가장 HP가 많은 적군
	SurveyGameCharacterInfo Enemy_HPRateHighest;
	// 가장 HP가 적은 적군
	SurveyGameCharacterInfo Enemy_HPRateNearest;

public:	
	USurveyInfo();
	virtual ~USurveyInfo() override;
public:
	void AddInfo(AGameCharacter* const pOwner, const FHitResult& HitResult);
	const SurveyGameCharacterInfo& GetSurveyTargetInfo(const ESurveyTargetTypes& targetType) const;
	void Clear();

};
