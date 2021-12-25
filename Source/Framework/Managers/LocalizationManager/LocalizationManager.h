#pragma once
#include "DataLoadLib/ManagerBase/LZManagerBase.h"
#include "Framework/Managers/IManager.h"

class FRAMEWORK_API LocalizationManager : public LZManagerBase, IManagers
{
public:
	LocalizationManager();
	virtual ~LocalizationManager() override;

	virtual bool Init() override;
};
