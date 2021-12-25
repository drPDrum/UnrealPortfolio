// Fill out your copyright notice in the Description page of Project Settings.


#include "StatController.h"

#include "Stat.h"
#include "Framework/Contents/GameCharacter/GameCharacter.h"

UStatController::UStatController()
{
	m_pOwner = nullptr;
	m_Stats.SetNum(static_cast<int>(EStatTypes::MAX));
}

UStatController::~UStatController()
{
	m_pOwner = nullptr;
}

void UStatController::Init(AGameCharacter* owner)
{
	m_pOwner = owner;
	if(owner)
	{
		SetStats(owner->GetStatData());
	}
}

AGameCharacter* UStatController::GetOwner()
{
	return m_pOwner;
}

void UStatController::SetStats(const CStatData* pStatData)
{
	Stat::SetStats(m_Stats, pStatData);
}

void UStatController::AddStats(const CStatData* pStatData)
{
	Stat::AddStats(m_Stats, pStatData);
}

void UStatController::SubStats(const CStatData* pStatData)
{
	Stat::SubStats(m_Stats, pStatData);
}

void UStatController::SetStats(const TArray<CStatData*>& statDatas)
{
	Stat::SetStats(m_Stats, statDatas);
}

void UStatController::AddStats(const TArray<CStatData*>& statDatas)
{
	Stat::AddStats(m_Stats, statDatas);
}

void UStatController::SubStats(const TArray<CStatData*>& statDatas)
{
	Stat::SubStats(m_Stats, statDatas);
}

void UStatController::SetStats(const TArray<Stat>& stats)
{
	m_Stats = stats;
}

void UStatController::AddStats(const TArray<Stat>& stats)
{
	Stat::AddStats(m_Stats, stats);
}

void UStatController::SubStats(const TArray<Stat>& stats)
{
	Stat::SubStats(m_Stats, stats);
}

TArray<Stat>& UStatController::GetStats()
{
	return m_Stats;
}

Stat& UStatController::GetStat(EStatTypes statType)
{
	return m_Stats[static_cast<int>(statType)];
}

const int&  UStatController::GetStatValue(EStatTypes statType)
{
	return GetStat(statType).GetTotalValue();
}

const int& UStatController::GetCrrHP()
{
	return m_CrrHP;
}

void UStatController::SetCrrHP(const int& hpValue)
{
	m_CrrHP = hpValue;
	if (m_CrrHP <= 0)
		m_CrrHP = 0;
}

void UStatController::AddCrrHP(const int& hpValue)
{
	m_CrrHP += hpValue;
	if (m_CrrHP <= 0)
		m_CrrHP = 0;
}

void UStatController::SubCrrHP(const int& hpValue)
{
	m_CrrHP -= hpValue;
	if (m_CrrHP <= 0)
		m_CrrHP = 0;
}

float UStatController::GetCrrHPRate()
{
	auto MaxHP = GetStatValue(EStatTypes::MaxHP);
	if (MaxHP == 0)
		return 0.f;
	
	return static_cast<float>(m_CrrHP) / static_cast<float>(MaxHP);
}

void UStatController::SetCrrHPRate(const float& hpRate)
{
	m_CrrHP = hpRate * GetStatValue(EStatTypes::MaxHP);
}

void UStatController::AddCrrHPRate(const float& hpRate)
{
	m_CrrHP += hpRate * GetStatValue(EStatTypes::MaxHP);
}

void UStatController::SubCrrHPRate(const float& hpRate)
{
	m_CrrHP -= hpRate * GetStatValue(EStatTypes::MaxHP);
}