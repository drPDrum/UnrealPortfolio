#include "WorldManager.h"

WorldManager::SLevelExeInfo::SLevelExeInfo()
{
	ExeType = ELevelExeType::None;
}

WorldManager::SLevelExeInfo::SLevelExeInfo(const ELevelExeType& eExeType, const FString& strLevelName)
{
	ExeType = eExeType;
	LevelName = strLevelName;
}
