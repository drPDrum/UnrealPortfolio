#pragma once
#include "../IManager.h"
#include "DataLoadLib/ManagerBase/DataFileManagerBase.h"
#include "DataLoadLib/DataFile/DataFileHeader.h"

enum class EDataFileEnum : uint8;
class CConstData;

class FRAMEWORK_API DataFileManager : public DataFileManagerBase, IManagers
{
private:
	TMap<uint32, TArray<class UActionData*>>	m_mapAllActionData;

public:
	DataFileManager();
	virtual ~DataFileManager() override;

	virtual bool Init() override;

	void GetActionDatas(uint32 nDataID, TArray<UActionData*>& result);
};
