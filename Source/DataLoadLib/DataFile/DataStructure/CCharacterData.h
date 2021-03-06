// Generate By DataTool. 2021-12-25 오전 7:02:26
// Drum.

#pragma once
#include "../CDataFileBase.h"

class DATALOADLIB_API CCharacterData : public CDataFileBase
{
public:
	FString Name;
	FString RefPath;
	FString FileNameModel;
	FString FileNameAnimInst;
	FString FileNameAIBT;
	int32 DataIdStat;
	int32 Height;
	int32 Radius;
	int32 Sight;

	virtual void SetInfo(const struct FRowDataInfo& fInfo) override;

	static EDataFileEnum GetEnum();
};
