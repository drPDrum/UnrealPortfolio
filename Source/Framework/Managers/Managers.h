#pragma once
#include "ClientAssetManager/ClientAssetManager.h"
#include "DataFileManager/DataFileManager.h"
#include "Framework/UEBaseClass/FrameworkGameInst.h"
#include "FXManager/FXManager.h"
#include "LocalizationManager/LocalizationManager.h"
#include "SFXManager/SFXManager.h"
#include "UIManager/UIManager.h"
#include "WorldManager/WorldManager.h"

class FRAMEWORK_API Managers
{
private:
	static bool m_bInit;

	static UFrameworkGameInst*			m_pGameInst;
										
	static WorldManager*				m_pWorldMgr;
	static ClientAssetManager*			m_pAssetMgr;
	static DataFileManager*				m_pDataMgr;
	static LocalizationManager*			m_pLZMgr;
	static FXManager*					m_pFXMgr;
	static SFXManager*					m_pSFXMgr;
	static UIManager*					m_pUIMgr;
		
public:
	static UFrameworkGameInst* const&	GameInst;

	static WorldManager* const&			World;
	static ClientAssetManager* const&	Asset;
	static DataFileManager* const&		Data;
	static LocalizationManager* const&	LZ;
	static FXManager* const&			FX;
	static SFXManager* const&			SFX;
	static UIManager* const&			UI;

	static bool IsInit();
	
	static void Init(UWorld* pWorld);
	static void Release();

	static void OnPrevChangeWorld();
	static void OnPostChangeWorld();

private:
	template <class T = IManagers>
	static void Init(T*& cManager);
	template <class T = IManagers>
	static void Release(T*& cManager);

private:
	Managers() {}
	~Managers() {}
};
