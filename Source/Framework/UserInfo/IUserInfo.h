#pragma once

class FRAMEWORK_API IUserInfo
{
public:
	virtual ~IUserInfo() { }

	virtual bool Init() { return true; }	
};
