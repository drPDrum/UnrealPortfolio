// Generate By DataTool. 2021-12-25 오전 7:02:26
// Drum.

#pragma once
#include "../CDataFileBase.h"

class DATALOADLIB_API CHelpCategoryData : public CDataFileBase
{
public:
	FString Name;

	virtual void SetInfo(const struct FRowDataInfo& fInfo) override;

	static EDataFileEnum GetEnum();
};
