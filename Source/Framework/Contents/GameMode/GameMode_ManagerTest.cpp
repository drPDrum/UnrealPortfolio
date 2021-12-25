#include "GameMode_ManagerTest.h"

#include "Framework/Contents/UI/Intro/Window_Intro.h"
#include "Framework/Managers/Managers.h"

AGameMode_ManagerTest::AGameMode_ManagerTest()
{
	//LOG(TEXT("%s"), *result.GetActor()->GetName());
	LOG(TEXT("ATestFrameworkGameModeBase::Initializer"));

	PrimaryActorTick.bCanEverTick = true;
}

void AGameMode_ManagerTest::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LOG(TEXT("ATestFrameworkGameModeBase::InitGame(%s, %s, %s)"), *MapName, *Options, *ErrorMessage);
}

void AGameMode_ManagerTest::BeginPlay()
{
	Super::BeginPlay();
	
	LOG(TEXT("ATestFrameworkGameModeBase::BeginPlay"));
}

void AGameMode_ManagerTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OnTickDebugInputHandler();
}

void AGameMode_ManagerTest::OnInit()
{
	TestCode();

	Managers::UI->OpenWindow<UWindow_Intro>();	
	Super::OnInit();
}

void AGameMode_ManagerTest::OnStartGame()
{
	Super::OnStartGame();
}

void AGameMode_ManagerTest::OnEndGame()
{
	Super::OnEndGame();
}

void AGameMode_ManagerTest:: PrintFPaths()
{
	LOG(TEXT("PrintFPaths"));
	LOG(TEXT("Root Dir : [%s]"), *FPaths::RootDir());
	LOG(TEXT("Engine Dir : [%s], [%s]"), *FPaths::EngineDir(), *FPaths::ConvertRelativePathToFull(*FPaths::EngineDir()));

	LOG(TEXT("Engine Contents Dir : [%s], [%s]"), *FPaths::ProjectContentDir(), *FPaths::ConvertRelativePathToFull(*FPaths::ProjectContentDir()));
	LOG(TEXT("Project Contents Dir : [%s], [%s]"), *FPaths::ProjectContentDir(), *FPaths::ConvertRelativePathToFull(*FPaths::ProjectContentDir()));

	LOG(TEXT("Project Dir : [%s]"), *FPaths::ProjectDir());
	LOG(TEXT("ProjectUser Dir : [%s]"), *FPaths::ProjectUserDir());
}

void AGameMode_ManagerTest::TestCode()
{
	TestCodeManagers();
	// TestCodeDataManager();
	// TestCodeLocalizationManager();
}

void AGameMode_ManagerTest::TestCodeManagers()
{
	if(!Managers::IsInit())
		Managers::Init(GetWorld());

	TestCodeDataManager();
	TestCodeLocalizationManager();
	TestCodeUIManager();
	
	// Managers::Release();
}

void AGameMode_ManagerTest::TestCodeDataManager()
{
	auto& pMgr = Managers::Data;
	
	const CCharacterData* cData = pMgr->GetData<CCharacterData>(1);
	
	if(cData != nullptr)
	{
		//cData->Name = TEXT("dd");
    	LOG(TEXT("Key : %d Name : %s RefPath : %s Height : %d"),
    		cData->ID, *cData->Name, *cData->RefPath, cData->Height);
		
    	auto& id = typeid(cData);
    	FString name = id.name();
    	FString RawName = id.raw_name();
    	LOG(TEXT("Type HashCode : %d Type Name : %s Raw Name : %s"), (int)id.hash_code(), *name, *RawName);	
	}

	const TMap<int32, CItemData*>* mapData = pMgr->GetMapData<CItemData>();
	if(mapData != nullptr)
	{
		for (auto& tuple : *mapData)
		{
			LOG(TEXT("Key : %d Name : %s ImgNameIcon : %s"),
				tuple.Value->ID, *tuple.Value->Name, *tuple.Value->ImgNameIcon);
		}
	}
	
	int nValue = pMgr->ConstData->INT_TEST;
	const FString& strValue = pMgr->ConstData->STR_TEST;
	LOG(TEXT("ConstData Test INT : %d STRING : %s"), nValue, *strValue);	
}

void AGameMode_ManagerTest::TestCodeLocalizationManager()
{
	auto& pMgr = Managers::LZ;
	auto& cMgr = *pMgr;

	FString strKey = TEXT("CODE_SAVE");
	LOG(TEXT("Localization Test KEY[%s] KOR : '%s' ENG : '%s'"),
		*strKey, *cMgr[strKey], *cMgr.Get(strKey, ELanguage::English));

	strKey = "CODE_YES";
	LOG(TEXT("Localization Test KEY[%s] KOR : '%s' ENG : '%s'"),
		*strKey, *cMgr[strKey], *cMgr.Get(strKey, ELanguage::English));
}

void AGameMode_ManagerTest::TestCodeUIManager()
{
	DataFileManager* pDataMgr = Managers::Data;

	// How to use single row data
	const CCharacterData* cData = pDataMgr->GetData<CCharacterData>(1);
	// How to use map data
	const TMap<int32, CItemData*>* mapData = pDataMgr->GetMapData<CItemData>();

	// How to use Const Data
	int nValue = pDataMgr->ConstData->INT_TEST;
	const FString& strValue = pDataMgr->ConstData->STR_TEST;
	
	LocalizationManager& cLZMgr = *Managers::LZ;

	// How to use Localization Data
	const FString& strLZValue = cLZMgr["CODE_YES"];
}

void AGameMode_ManagerTest::TestCodePopup(int32 nTypeID, bool bUseBasicButton)
{
	LOG(TEXT("TestCodePopup TypeID [%d] BasicButton ? [%s]"),
		nTypeID, bUseBasicButton ? TEXT("true") : TEXT("false"));
	
	auto& cMgr = *Managers::LZ;
	switch (nTypeID)
	{
		case 1:
			if(bUseBasicButton)
			{
				Managers::UI->Popup(cMgr["POPUP_TITLE_TEST1"],
					cMgr["POPUP_CONTENTS_1"],
					[this]()
					{
						OnOkCallback(1);
					});
			}
			else
			{
				Managers::UI->Popup(cMgr["POPUP_TITLE_TEST1"],
					cMgr["POPUP_CONTENTS_BTN_1"],
					cMgr["POPUP_BTN_OK_1"],
					[this]()
					{
						OnOkCallback(1);
					});
			}
			break;
		case 2:
			if(bUseBasicButton)
			{
				Managers::UI->Popup(cMgr["POPUP_TITLE_TEST2"],
					cMgr["POPUP_CONTENTS_2"],
					[this]()
					{
						OnOkCallback(2);
					},
					[this]()
					{
						OnCancelCallback(2);
					});
			}
			else
			{
				Managers::UI->Popup(cMgr["POPUP_TITLE_TEST2"],
					cMgr["POPUP_CONTENTS_BTN_2"],
					cMgr["POPUP_BTN_OK_2"],
					[this]()
					{
						OnOkCallback(2);
					},
					cMgr["POPUP_BTN_CANCEL_2"],
					[this]()
					{
						OnCancelCallback(2);
					});
			}
			break;

		case 3:
			if(bUseBasicButton)
			{
				Managers::UI->Popup(cMgr["POPUP_TITLE_TEST2"],
					cMgr["POPUP_CONTENTS_2"],
					[this]()
					{
						OnOkCallback(3);
					},
					[this]()
					{
						OnNoCallback(3);
					},
					[this]()
					{
						OnCancelCallback(3);
					});
			}
			else
			{
				Managers::UI->Popup(cMgr["POPUP_TITLE_TEST3"],
					cMgr["POPUP_CONTENTS_BTN_3"],
					cMgr["POPUP_BTN_OK_3"],
					[this]()
					{
						OnOkCallback(3);
					},
					cMgr["POPUP_BTN_NO_3"],
					[this]()
					{
						OnNoCallback(3);
					},
					cMgr["POPUP_BTN_CANCEL_3"],
					[this]()
					{
						OnCancelCallback(3);
					});
			}
			break;
	}
}

void AGameMode_ManagerTest::OnOkCallback(int32 nValue)
{
	LOG(TEXT("OK Callback : %d"), nValue);	
}

void AGameMode_ManagerTest::OnCancelCallback(int32 nValue)
{
	LOG(TEXT("Cancel Callback : %d"), nValue);
}

void AGameMode_ManagerTest::OnNoCallback(int32 nValue)
{
	LOG(TEXT("No Callback : %d"), nValue);
}




