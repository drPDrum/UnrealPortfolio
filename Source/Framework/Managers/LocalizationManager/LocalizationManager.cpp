#include "LocalizationManager.h"
#include "DataLoadLib/DataFile/CDataFileFactory.h"
#include "Framework/Managers/Managers.h"

LocalizationManager::LocalizationManager()
	: LZManagerBase()
{	
}

LocalizationManager::~LocalizationManager()
{
}

bool LocalizationManager::Init()
{
	if(!IManagers::Init())
		return false;

	LoadAllData();

	return true;
}