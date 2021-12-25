#pragma once

class FRAMEWORK_API IManagers
{
public:
	virtual ~IManagers() { }

	virtual bool Init() { return true; }	
};
