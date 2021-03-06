// Generate By DataTool. 2021-12-25 오전 7:02:26
// Drum.

#include "CLandData.h"

void CLandData::SetInfo(const FRowDataInfo& fInfo)
{
	CDataFileBase::SetInfo(fInfo);

	WorldId = static_cast<EWorldId>(FCString::Atoi(*fInfo.arrColData[1]));
	LandName = fInfo.arrColData[2];
	NeighberLandName = fInfo.arrColData[3];
}

EDataFileEnum CLandData::GetEnum()
{
	return EDataFileEnum::LandData;
}
