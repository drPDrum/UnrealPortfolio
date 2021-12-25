#include "Window_GameMain.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Framework/Contents/UI/Common/Module/UIModule_Help.h"
#include "Framework/Managers/Managers.h"
#include "Module/UIModule_Inventory.h"

UWindow_GameMain::UWindow_GameMain(const FObjectInitializer& ObjectInitializer)
	: UWindowBase(ObjectInitializer)
{
	bIsFocusable = true;
	m_bUseMouse = false;
}

void UWindow_GameMain::OnWindowCreate()
{
	Super::OnWindowCreate();

	m_pCanvasCenterRoot = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Canvas_CenterRoot")));
	m_pCanvasLeftRoot = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Canvas_LeftRoot")));
	m_pCanvasRightRoot = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("Canvas_RightRoot")));

	m_pBtnInventory = Cast<UButton>(GetWidgetFromName(TEXT("btnInventory")));
	m_pBtnHelp = Cast<UButton>(GetWidgetFromName(TEXT("btnHelp")));

	m_pBtnHelp->OnClicked.AddDynamic(this, &UWindow_GameMain::OnClickHelp);
	m_pBtnInventory->OnClicked.AddDynamic(this, &UWindow_GameMain::OnClickInventory);
}

bool UWindow_GameMain::IsLocked()
{
	return Super::IsLocked();
}

void UWindow_GameMain::InitHotkey()
{
	AddKey_PlayerCtrl();
	AddKey(EKeys::One, this, &UWindow_GameMain::OnClickHelp);
	AddKey(EKeys::Two, this, &UWindow_GameMain::OnClickInventory);
	AddKey(EKeys::LeftControl, this, &UWindow_GameMain::OnKeyDownShowMouse);
}

void UWindow_GameMain::OnClickInventory()
{
	if(IsOpenedUIModule<UUIModule_Inventory>())
	{
		auto* pModule = GetUIModule<UUIModule_Inventory>(false);
		if(pModule == nullptr)
			CloseUIModule<UUIModule_Inventory>();

		auto eViewType = pModule->ChangeNextViewType();
		if(eViewType == EInvenViewTypes::None)
			CloseUIModule<UUIModule_Inventory>();
	}
	else
		OpenUIModule<UUIModule_Inventory>(m_pCanvasRightRoot);
}

void UWindow_GameMain::OnClickHelp()
{
	if(IsOpenedUIModule<UUIModule_Help>())
		CloseUIModule<UUIModule_Help>();
	else
		OpenUIModule<UUIModule_Help>(m_pCanvasLeftRoot);
}

void UWindow_GameMain::OnKeyDownShowMouse()
{
	Managers::UI->GetInputCtrl()->SetMouseEnable(true);
}
