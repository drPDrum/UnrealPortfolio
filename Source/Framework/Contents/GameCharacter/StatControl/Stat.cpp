#include "Stat.h"
#include "../GameCharacter_Define.h"
#include "DataLoadLib/DataFile/DataStructure/CStatData.h"

Stat::Stat() : _value(0), _rate(0), _totalValue(0)
{
}

Stat::~Stat()
{
}

const int& Stat::GetValue()
{
	return _value;
}

void Stat::SetValue(const int& value)
{
	_value = value;
}

const int& Stat::GetRate()
{
	return _rate;
}

void Stat::SetRate(const int& rate)
{
	_rate = rate;
}

const int& Stat::GetTotalValue()
{
	return _totalValue;
}

void Stat::Refresh()
{
	_totalValue = FMath::CeilToInt(_value * ((10000 + _rate) * 0.0001f));
}

void Stat::Clear()
{
	_value = 0;
	_rate = 0;
	_totalValue = 0;
}

TArray<Stat> Stat::ConvertFromStatData(const TArray<CStatData*>& statDatas)
{
	TArray<Stat> result;
	result.SetNum(static_cast<int>(EStatTypes::MAX));

	if (statDatas.Num() == 0)
		return result;

	for (const auto statData : statDatas)
	{
		if (statData)
			AddStats(result, statData);
	}

	for (auto& stat : result)
		stat.Refresh();

	return result;
}

TArray<Stat> Stat::ConvertFromStatData(const CStatData* const statData)
{
	TArray<Stat> result;
	result.SetNum(static_cast<int>(EStatTypes::MAX));

	if (!statData)
		return result;

	AddStats(result, statData);

	for (auto& stat : result)
		stat.Refresh();

	return result;
}

void Stat::SetStats(TArray<Stat>& stats, const CStatData* const statData)
{
	stats.SetNum(static_cast<int>(EStatTypes::MAX));
	for (auto& stat : stats)
		stat.Clear();

	AddStats(stats, statData);
}

void Stat::AddStats(TArray<Stat>& stats, const CStatData* const statData)
{
	if (!statData)
		return;

	stats.SetNum(static_cast<int>(EStatTypes::MAX));

	// 이하 스탯 추가시 작업 필요
	
	auto& statAtk		= stats[static_cast<int>(EStatTypes::ATK)];
	statAtk._value		+= statData->Atk;
	statAtk._rate		+= statData->AtkRate;
	statAtk.Refresh();

	auto& statMaxHp		= stats[static_cast<int>(EStatTypes::MaxHP)];
	statMaxHp._value	+= statData->MaxHp;
	statMaxHp._rate		+= statData->MaxHpRate;
	statMaxHp.Refresh();
}

void Stat::SubStats(TArray<Stat>& stats, const CStatData* const statData)
{
	if (!statData)
		return;

	stats.SetNum(static_cast<int>(EStatTypes::MAX));

	// 이하 스탯 추가시 작업 필요
	
	auto& statAtk		= stats[static_cast<int>(EStatTypes::ATK)];
	statAtk._value		-= statData->Atk;
	statAtk._rate		-= statData->AtkRate;
	statAtk.Refresh();

	auto& statMaxHp		= stats[static_cast<int>(EStatTypes::MaxHP)];
	statMaxHp._value	-= statData->MaxHp;
	statMaxHp._rate		-= statData->MaxHpRate;
	statMaxHp.Refresh();
}

void Stat::SetStats(TArray<Stat>& stats, const TArray<CStatData*>& statDatas)
{
	stats.SetNum(static_cast<int>(EStatTypes::MAX));
	for (auto& stat : stats)
		stat.Clear();

	AddStats(stats, statDatas);
}

void Stat::AddStats(TArray<Stat>& stats, const TArray<CStatData*>& statDatas)
{
	for (auto pStatData : statDatas)
		AddStats(stats, pStatData);
}

void Stat::SubStats(TArray<Stat>& stats, const TArray<CStatData*>& statDatas)
{
	for (auto pStatData : statDatas)
		SubStats(stats, pStatData);
}

void Stat::AddStats(TArray<Stat>& dstStats,const TArray<Stat>& srcStats)
{
	const auto& nStatTypeCount = static_cast<int>(EStatTypes::MAX);
	if (dstStats.Num() < nStatTypeCount || srcStats.Num() < nStatTypeCount)
		return;

	for (int i = 0 ; i < nStatTypeCount ; ++i)
	{
		auto& dst = dstStats[i];
		auto& src = srcStats[i];

		dst._value = dst._value + src._value;
		dst._rate = dst._rate + src._rate;

		dst.Refresh();
	}
}

void Stat::SubStats(TArray<Stat>& dstStats,const TArray<Stat>& srcStats)
{
	const auto& nStatTypeCount = static_cast<int>(EStatTypes::MAX);
	if (dstStats.Num() < nStatTypeCount || srcStats.Num() < nStatTypeCount)
		return;

	for (int i = 0 ; i < nStatTypeCount ; ++i)
	{
		auto& dst = dstStats[i];
		auto& src = srcStats[i];

		dst._value = dst._value - src._value;
		dst._rate = dst._rate - src._rate;

		dst.Refresh();
	}
}