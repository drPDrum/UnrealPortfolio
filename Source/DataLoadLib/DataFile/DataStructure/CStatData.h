// Generate By DataTool. 2021-12-25 오전 7:02:26
// Drum.

#pragma once
#include "../CDataFileBase.h"

class DATALOADLIB_API CStatData : public CDataFileBase
{
public:
	int32 Atk;
	int32 AtkRate;
	int32 MaxHp;
	int32 MaxHpRate;

	virtual void SetInfo(const struct FRowDataInfo& fInfo) override;

	static EDataFileEnum GetEnum();
};
