#pragma once

class RuntimeUtils : public IModuleInterface
{
public:	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


