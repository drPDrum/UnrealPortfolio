// Generate By DataTool. 2021-12-25 오전 7:02:27
// Drum.

#include "CConstData.h"
#include "DataLoadLib/GlobalDataStruct.h"

void CConstData::SetInfo(const TArray<FRowDataInfo>& TableInfo)
{
	INT_TEST = FCString::Atoi(*TableInfo[0].arrColData[2]);
	STR_TEST = TableInfo[1].arrColData[3];
	BP_MODAL = TableInfo[2].arrColData[3];
	LOADING_COMMON = TableInfo[3].arrColData[3];
}
