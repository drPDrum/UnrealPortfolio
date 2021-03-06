// Generate By DataTool. 2021-12-25 오전 7:02:31
// Drum
#pragma once
#include "DataFileHeader.h"

class DATALOADLIB_API CDataFileContainer
{
	friend class CDataFileFactory;

private:
	CConstData* ConstData;
	TMap<int32, CCharacterData*> MapCharacterData;
	TMap<int32, CItemData*> MapItemData;
	TMap<int32, CHelpData*> MapHelpData;
	TMap<int32, CHelpCategoryData*> MapHelpCategoryData;
	TMap<int32, CLandData*> MapLandData;
	TMap<int32, CStatData*> MapStatData;
	TMap<FString, CLocalizationData*> MapLocalizationData;
};
