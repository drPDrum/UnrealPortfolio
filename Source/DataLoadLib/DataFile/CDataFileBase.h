// Generate By DataTool. 2021-12-25 오전 7:02:26
// Generated only one time when file is not exists.
// Drum
#pragma once

#include "DataFileEnum.h"
#include "DataLoadLib/GlobalDataStruct.h"
#include "DataStructure/DataTypeEnum.h"

class DATALOADLIB_API CDataFileBase
{
public:
	int32 ID;

	virtual ~CDataFileBase() { }

	virtual void SetInfo(const FRowDataInfo& fInfo);

	static EDataFileEnum GetEnum();
};
