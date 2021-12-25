#pragma once
#include "IUserInfo.h"

class UserAccountInfo;
class UserCharacterInfo;

class FRAMEWORK_API UserInfo
{
private:
	static bool m_bInit;

	static UserAccountInfo*				m_pAccInfo;
	static UserCharacterInfo*			m_pCharInfo;
		
public:
	static UserAccountInfo* const&		Account;
	static UserCharacterInfo* const&	Character;
	
	static bool IsInit();
	
	static void Init();
	static void Release();

private:
	template <class T = IUserInfo>
	static void Init(T*& pInfo);
	template <class T = IUserInfo>
	static void Release(T*& pInfo);

private:
	UserInfo() {}
	~UserInfo() {}
};
