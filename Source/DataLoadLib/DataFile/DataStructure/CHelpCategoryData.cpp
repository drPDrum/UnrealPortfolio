// Generate By DataTool. 2021-12-25 오전 7:02:26
// Drum.

#include "CHelpCategoryData.h"

void CHelpCategoryData::SetInfo(const FRowDataInfo& fInfo)
{
	CDataFileBase::SetInfo(fInfo);

	Name = fInfo.arrColData[1];
}

EDataFileEnum CHelpCategoryData::GetEnum()
{
	return EDataFileEnum::HelpCategoryData;
}
