#include "UIModule_Inventory.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/ListView.h"
#include "Components/TileView.h"
#include "Framework/UserInfo/UserInfo.h"
#include "Framework/UserInfo/Character/UserCharacterInfo.h"

void UUIModule_Inventory::OnUIModuleOpened(UWindowBase* MovieSceneBlends)
{
	Super::OnUIModuleOpened(MovieSceneBlends);

	// 리스트 뷰 초기화
	m_pPanelListView = Cast<UCanvasPanel>(GetWidgetFromName((TEXT("CanvasPanel_ListView"))));
	m_pListView = Cast<UListView>(GetWidgetFromName((TEXT("ListView_Inventory"))));
	m_pListView->OnItemClicked().AddUObject(this, &UUIModule_Inventory::OnClickListItem);
	m_pBtnListView = Cast<UButton>(GetWidgetFromName((TEXT("btn_ListView"))));
	m_pBtnListView->OnClicked.AddDynamic(this, &UUIModule_Inventory::OnClickChangeView);

	// 타일 뷰 초기화
	m_pPanelTileView = Cast<UCanvasPanel>(GetWidgetFromName((TEXT("CanvasPanel_TileView"))));
	m_pTileView = Cast<UTileView>(GetWidgetFromName((TEXT("TileView_Inventory"))));
	m_pTileView->OnItemClicked().AddUObject(this, &UUIModule_Inventory::OnClickListItem);
	m_pBtnTileView = Cast<UButton>(GetWidgetFromName((TEXT("btn_TileView"))));
	m_pBtnTileView->OnClicked.AddDynamic(this, &UUIModule_Inventory::OnClickChangeView);

	// 데이터 세팅
	auto mapItems = UserInfo::Character->GetInventory();
	for (auto& tuple : mapItems)
	{
		auto* pUScrollListData = NewObject<UViewData_Inventory>(this);
		pUScrollListData->SetData(tuple.Value);
		
		m_pListView->AddItem(pUScrollListData);
		m_pTileView->AddItem(pUScrollListData);		
	}

	// 기본은 List로 보여준다.
	m_eVurViewType = EInvenViewTypes::None;
	ChangeViewType(EInvenViewTypes::List);
}

void UUIModule_Inventory::OnClickListItem(UObject* pViewData)
{
	const auto& pViewDataInven = Cast<UViewData_Inventory>(pViewData);
	const auto& pUserData = pViewDataInven->GetData();
	const auto& pItemData = pUserData->GetData();

	LOG_WARNING(TEXT("Clicked Item : %s Count : %d"), *pItemData->Name, pUserData->GetStackCount());
}

void UUIModule_Inventory::OnClickChangeView()
{
	switch (m_eVurViewType)
	{
		case EInvenViewTypes::List: ChangeViewType(EInvenViewTypes::Tile);	break;
		case EInvenViewTypes::Tile: ChangeViewType(EInvenViewTypes::List);	break;
	}
}

EInvenViewTypes UUIModule_Inventory::ChangeNextViewType()
{
	switch (m_eVurViewType)
	{
		case EInvenViewTypes::None: ChangeViewType(EInvenViewTypes::List);	break;
		case EInvenViewTypes::List: ChangeViewType(EInvenViewTypes::Tile);	break;
		case EInvenViewTypes::Tile: ChangeViewType(EInvenViewTypes::None);	break;
	}

	return m_eVurViewType;
}

void UUIModule_Inventory::ChangeViewType(const EInvenViewTypes& eViewType)
{
	if(m_eVurViewType == eViewType)
		return;

	switch (eViewType)
	{
		case EInvenViewTypes::List:
			m_pPanelListView->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			m_pPanelTileView->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case EInvenViewTypes::Tile:
			m_pPanelListView->SetVisibility(ESlateVisibility::Collapsed);
			m_pPanelTileView->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		default:
			m_pPanelListView->SetVisibility(ESlateVisibility::Collapsed);
			m_pPanelTileView->SetVisibility(ESlateVisibility::Collapsed);
			break;
	}

	m_eVurViewType = eViewType;
}
