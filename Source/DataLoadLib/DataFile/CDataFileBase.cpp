// Generate By DataTool. 2021-12-25 오전 7:02:26
// Generated only one time when file is not exists.
// Drum
#include "CDataFileBase.h"

void CDataFileBase::SetInfo(const FRowDataInfo& fInfo)
{
	ID = FCString::Atoi(*fInfo.arrColData[0]);
}

EDataFileEnum CDataFileBase::GetEnum()
{
	return EDataFileEnum::MAX;
}
