// Generate By DataTool. 2021-12-25 오전 7:02:31
// Generated only one time when file is not exists.
// Drum
#include "CLocalizationData.h"
#include "DataFileEnum.h"
#include "DataLoadLib/GlobalDataStruct.h"

FString CLocalizationData::KEY_EMPTY = TEXT("NONE");

void CLocalizationData::SetInfo(const FRowDataInfo& fInfo)
{
	ID = fInfo.arrColData[0];
	Korean = fInfo.arrColData[static_cast<int32>(ELanguage::Korean)];
	English = fInfo.arrColData[static_cast<int32>(ELanguage::English)];
	Japanese = fInfo.arrColData[static_cast<int32>(ELanguage::Japanese)];
}

const FString& CLocalizationData::GetLanguage(int nIndex) const
{
	return GetLanguage(static_cast<ELanguage>(nIndex));
}

const FString& CLocalizationData::GetLanguage(ELanguage eLanguage) const
{
	switch (eLanguage)
	{
	case ELanguage::Korean: return Korean;
	case ELanguage::English: return English;
	case ELanguage::Japanese: return Japanese;
	}

	return KEY_EMPTY;
}
