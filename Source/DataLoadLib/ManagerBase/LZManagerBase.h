#pragma once
#include "DataLoadLib/DataFile/DataFileEnum.h"

class CLocalizationData;
class DATALOADLIB_API LZManagerBase
{
protected:
	const TMap<FString, CLocalizationData*>*	m_mapAllData;
	class CDataFileFactory*						m_pDataFactory;
	ELanguage									m_eCurLanguage;
	
public:
	LZManagerBase();
	virtual ~LZManagerBase();

	virtual void LoadAllData();

	void ChangeLanguage(ELanguage eLanguage);

	const FString& Get(const FString& strKey, ELanguage eLanguage = ELanguage::None) const;	

	const FString& operator[](const FString& strKey) const;	
};
