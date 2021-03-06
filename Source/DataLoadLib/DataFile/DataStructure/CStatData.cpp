// Generate By DataTool. 2021-12-25 오전 7:02:26
// Drum.

#include "CStatData.h"

void CStatData::SetInfo(const FRowDataInfo& fInfo)
{
	CDataFileBase::SetInfo(fInfo);

	Atk = FCString::Atoi(*fInfo.arrColData[1]);
	AtkRate = FCString::Atoi(*fInfo.arrColData[2]);
	MaxHp = FCString::Atoi(*fInfo.arrColData[3]);
	MaxHpRate = FCString::Atoi(*fInfo.arrColData[4]);
}

EDataFileEnum CStatData::GetEnum()
{
	return EDataFileEnum::StatData;
}
