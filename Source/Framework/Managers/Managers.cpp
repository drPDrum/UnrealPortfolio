#include "Managers.h"

#include "Framework/UserInfo/UserInfo.h"

bool Managers::m_bInit = false;

UFrameworkGameInst* Managers::m_pGameInst = nullptr;
UFrameworkGameInst* const& Managers::GameInst = m_pGameInst;

WorldManager* Managers::m_pWorldMgr = nullptr;
WorldManager* const& Managers::World = m_pWorldMgr;

ClientAssetManager* Managers::m_pAssetMgr = nullptr;
ClientAssetManager* const& Managers::Asset = m_pAssetMgr;

DataFileManager* Managers::m_pDataMgr = nullptr;
DataFileManager* const& Managers::Data = m_pDataMgr;

LocalizationManager* Managers::m_pLZMgr = nullptr;
LocalizationManager* const& Managers::LZ = m_pLZMgr;

FXManager* Managers::m_pFXMgr = nullptr;
FXManager* const& Managers::FX = m_pFXMgr;

SFXManager* Managers::m_pSFXMgr = nullptr;
SFXManager* const& Managers::SFX = m_pSFXMgr;

UIManager* Managers::m_pUIMgr = nullptr;
UIManager* const& Managers::UI = m_pUIMgr;

bool Managers::IsInit()
{
	return m_bInit;
}

void Managers::Init(UWorld* pWorld)
{
	if(m_bInit || pWorld == nullptr)
		return;

	m_bInit = true;
	
	m_pGameInst = Cast<UFrameworkGameInst>(pWorld->GetGameInstance());

	
	Init(m_pWorldMgr);		// 가장 먼저 World Manager를 Init 한다.
	m_pWorldMgr->RegistWorld(pWorld);
	
	Init(m_pAssetMgr);
	Init(m_pDataMgr);
	Init(m_pLZMgr);
	Init(m_pFXMgr);
	Init(m_pSFXMgr);
	Init(m_pUIMgr);			//Localization Manager 뒤에 호출

	//TODO : 서버로 부터 데이터 받을 시 해당 데이터로 초기화 한다.
	UserInfo::Init();
}

void Managers::Release()
{
	if(!m_bInit)
		return;

	m_bInit = false;

	UserInfo::Release();

	Release(m_pUIMgr);
	Release(m_pSFXMgr);
	Release(m_pFXMgr);
	Release(m_pLZMgr);
	Release(m_pDataMgr);
	Release(m_pAssetMgr);	
	Release(m_pWorldMgr);	// 가장 마지막에 Release한다. 마지막까지 참조가 있을 수 있음
	
	m_pGameInst = nullptr;
}

template <class T>
void Managers::Init(T*& pManager)
{
	pManager = new T();
	
	if(!pManager->Init())
	{
		//TODO : Exception
	}
}

template <class T>
void Managers::Release(T*& pManager)
{
	if(pManager)
	{
		delete pManager;
		pManager = nullptr;
	}
}

void Managers::OnPrevChangeWorld()
{
	if (m_pSFXMgr)
		m_pSFXMgr->OnPrevChangeLevel();
	
	if (m_pFXMgr)
		m_pFXMgr->OnPrevChangeLevel();

}

void Managers::OnPostChangeWorld()
{
	if (m_pSFXMgr)
		m_pSFXMgr->OnPostChangeLevel();
	
	if (m_pUIMgr)
		m_pUIMgr->OnPostChangeLevel();
}