#include "UserInfo.h"
#include "UserInfo.h"

bool UserInfo::m_bInit = false;

UserAccountInfo* UserInfo::m_pAccInfo = nullptr;
UserAccountInfo* const& UserInfo::Account = m_pAccInfo;

UserCharacterInfo* UserInfo::m_pCharInfo = nullptr;
UserCharacterInfo* const& UserInfo::Character = m_pCharInfo;


bool UserInfo::IsInit()
{
	return m_bInit;
}

void UserInfo::Init()
{
	if(m_bInit)
		return;

	m_bInit = true;

	Init(m_pAccInfo);
	Init(m_pCharInfo);
}

void UserInfo::Release()
{
	if(!m_bInit)
		return;

	m_bInit = false;

	Release(m_pCharInfo);
	Release(m_pAccInfo);
}

template <class T>
void UserInfo::Init(T*& pData)
{
	pData = new T();
	
	if(!pData->Init())
	{
		//TODO : Exception
	}
}

template <class T>
void UserInfo::Release(T*& pData)
{
	if(pData)
	{
		delete pData;
		pData = nullptr;
	}
}