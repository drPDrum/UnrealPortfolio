#include "UIModule_Help.h"

#include "Components/MultiLineEditableText.h"
#include "Components/TreeView.h"
#include "Framework/Managers/Managers.h"

void UUIModule_Help::OnUIModuleCreate(UClass* pBaseUClass)
{
	Super::OnUIModuleCreate(pBaseUClass);

	m_pViewData_Help_Selected = nullptr;

	m_pTxtHelpContents = Cast<UMultiLineEditableText>(GetWidgetFromName(TEXT("CTxt_HelpContents")));
	m_pTreeView = Cast<UTreeView>(GetWidgetFromName("TreeView_Help"));

	auto& cMgr = *Managers::LZ;
	m_pTxtHelpContents->SetText(FText::FromString(cMgr["SELECT_ITEM"]));
	m_pTreeView->OnItemClicked().AddUObject(this, &UUIModule_Help::OnClickItem);
	m_pTreeView->SetOnGetItemChildren(this, &UUIModule_Help::GetItemChildren);

	// 도움말 데이터 준비
	TMap<int, TArray<const CHelpData*>> mapHelpDataByCategoryID;

	auto* mapHelpData = Managers::Data->GetMapData<CHelpData>();
	for (auto& pHelpData : *mapHelpData)
	{
		auto& nCategoryID = pHelpData.Value->CategoryDataId;
		
		if (!mapHelpDataByCategoryID.Contains(nCategoryID))
			mapHelpDataByCategoryID.Add(nCategoryID, TArray<const CHelpData*>());

		mapHelpDataByCategoryID[nCategoryID].Add(pHelpData.Value);
	}

	// 도움말 카테고리 데이터 세팅
	auto* mapHelpCategoryData = Managers::Data->GetMapData<CHelpCategoryData>();
	for (auto& pHelpCategoryData : *mapHelpCategoryData)
	{
		// 정작 도움말이 없다면 넘긴다.
		if (!mapHelpDataByCategoryID.Contains(pHelpCategoryData.Value->ID))
			continue;
		
		auto* pViewData_HelpCategory = NewObject<UViewData_HelpCategory>(this);
		pViewData_HelpCategory->SetData(pHelpCategoryData.Value, mapHelpDataByCategoryID[pHelpCategoryData.Value->ID]);
		
		m_pTreeView->AddItem(pViewData_HelpCategory);
	}
}

void UUIModule_Help::OnClickItem(UObject* pViewData)
{
	const auto& pViewData_Help = Cast<UViewData_Help>(pViewData);
	if(pViewData_Help)
	{
		auto& cMgr = *Managers::LZ;
		const auto& pData = pViewData_Help->GetData();
		m_pTxtHelpContents->SetText(FText::FromString(cMgr[pData->TxtContents]));

		if(m_pViewData_Help_Selected)
			m_pViewData_Help_Selected->SetSelected(false);

		m_pViewData_Help_Selected = pViewData_Help;
		m_pViewData_Help_Selected->SetSelected(true);
	}
}

void UUIModule_Help::GetItemChildren(UObject* pViewData, TArray<UObject*>& arrChildrenViewData)
{
	const auto& pViewData_Category = Cast<UViewData_HelpCategory>(pViewData);
	if(pViewData_Category)
		arrChildrenViewData = pViewData_Category->GetChildrenData();
}
