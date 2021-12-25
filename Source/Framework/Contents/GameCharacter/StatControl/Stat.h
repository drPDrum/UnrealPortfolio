#pragma once

class CStatData;

class FRAMEWORK_API Stat
{
private:
    int _value;            // 고정 수치
    int _rate;           // 버프등을 위해서 필요 / 0.001f 천분율
    int _totalValue;

public:
    Stat();
    ~Stat();

    const int& GetValue();
    void SetValue(const int& value);
    
    const int& GetRate();
    void SetRate(const int& rate);

    const int& GetTotalValue();

    void Refresh();
    void Clear();

    static TArray<Stat> ConvertFromStatData(const TArray<CStatData*>& statDatas);
    static TArray<Stat> ConvertFromStatData(const CStatData* const statData);

    static void SetStats(TArray<Stat>& stats, const CStatData* const statData);
    static void AddStats(TArray<Stat>& stats, const CStatData* const statData);
    static void SubStats(TArray<Stat>& stats, const CStatData* const statData);
    
    static void SetStats(TArray<Stat>& stats, const TArray<CStatData*>& statDatas);
    static void AddStats(TArray<Stat>& stats, const TArray<CStatData*>& statDatas);
    static void SubStats(TArray<Stat>& stats, const TArray<CStatData*>& statDatas);

    static void AddStats(TArray<Stat>& dstStats,const TArray<Stat>& srcStats);
    static void SubStats(TArray<Stat>& dstStats,const TArray<Stat>& srcStats);
};