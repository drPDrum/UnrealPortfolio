// Generate By DataTool. 2021-12-25 오전 7:02:26
// Drum.

#include "CHelpData.h"

void CHelpData::SetInfo(const FRowDataInfo& fInfo)
{
	CDataFileBase::SetInfo(fInfo);

	CategoryDataId = FCString::Atoi(*fInfo.arrColData[1]);
	Name = fInfo.arrColData[2];
	TxtContents = fInfo.arrColData[3];
}

EDataFileEnum CHelpData::GetEnum()
{
	return EDataFileEnum::HelpData;
}
