// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../CommonInfo.h"
#include "../GameCharacter_Define.h"
#include "Stat.h"
#include "StatController.generated.h"

class AGameCharacter;

UCLASS()
class FRAMEWORK_API UStatController : public UObject
{
	GENERATED_BODY()

private:	
	AGameCharacter* m_pOwner;

	TArray<Stat> m_Stats;

	int m_CrrHP;
	
public:
	UStatController();
	virtual ~UStatController() override;
	
	void Init(AGameCharacter* owner);
	AGameCharacter* GetOwner();

	void SetStats(const CStatData* pStatData);
	void AddStats(const CStatData* pStatData);
	void SubStats(const CStatData* pStatData);

	void SetStats(const TArray<CStatData*>& statDatas);
	void AddStats(const TArray<CStatData*>& statDatas);
	void SubStats(const TArray<CStatData*>& statDatas);

	void SetStats(const TArray<Stat>& statDatas);
	void AddStats(const TArray<Stat>& statDatas);
	void SubStats(const TArray<Stat>& statDatas);

	TArray<Stat>& GetStats();
	Stat& GetStat(EStatTypes statType);
	const int& GetStatValue(EStatTypes statType);

	const int& GetCrrHP();
	void SetCrrHP(const int& hpValue);
	void AddCrrHP(const int& hpValue);
	void SubCrrHP(const int& hpValue);

	float GetCrrHPRate();
	void SetCrrHPRate(const float& hpRate);
	void AddCrrHPRate(const float& hpRate);
	void SubCrrHPRate(const float& hpRate);
};