#pragma once
#include "DataLoadLib/DataFile/CDataFileFactory.h"

enum class EDataFileEnum : uint8;
class CConstData;

class DATALOADLIB_API DataFileManagerBase
{
protected:
	TMap<EDataFileEnum, void*>					m_mapAllData;

	CDataFileFactory*							m_pDataFactory;
	
public:
	const CConstData* 							ConstData;
	
	DataFileManagerBase();
	virtual ~DataFileManagerBase();

	virtual void LoadAllData();

	template<class TDataFile = CDataFileBase>
	const TDataFile* GetData(int32 nKey) const
	{
		if(m_pDataFactory == nullptr)
			return nullptr;
		
		const CDataFileBase* cData = m_pDataFactory->GetData(TDataFile::GetEnum(), nKey);
		if(cData == nullptr)
			return nullptr;

		return static_cast<const TDataFile*>(cData);
	}

	template <class TDataFile = CDataFileBase>
	const TMap<int32, TDataFile*>* GetMapData() const
	{
		if(m_pDataFactory == nullptr)
			return nullptr;

		EDataFileEnum eType = TDataFile::GetEnum();
		
		if(m_mapAllData.Contains(eType))
			return static_cast<const TMap<int32, TDataFile*>*>(m_mapAllData[eType]);
	
		return nullptr;
	}
};
