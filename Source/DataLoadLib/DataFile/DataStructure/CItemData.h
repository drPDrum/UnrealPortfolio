// Generate By DataTool. 2021-12-25 오전 7:02:26
// Drum.

#pragma once
#include "../CDataFileBase.h"

class DATALOADLIB_API CItemData : public CDataFileBase
{
public:
	FString Name;
	FString ImgNameIcon;

	virtual void SetInfo(const struct FRowDataInfo& fInfo) override;

	static EDataFileEnum GetEnum();
};
