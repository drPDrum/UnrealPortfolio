#include "UITreeViewItem_Help.h"

#include "UIModule_Help.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Framework/Managers/Managers.h"

void UUITreeViewItem_Help::NativeConstruct()
{
	Super::NativeConstruct();

	m_pCanvasHelpCategory = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_HelpCategory")));;
	m_pCanvasHelp = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_Help")));;
	m_pTxtHelpCategoryName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Ctxt_HelpCategoryName")));;
	m_pTxtHelpName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Ctxt_HelpName")));;
	m_pImgHelpBgSelected = Cast<UImage>(GetWidgetFromName(TEXT("img_HelpBg_Selected")));;
	m_pImgHelpBgUnselected = Cast<UImage>(GetWidgetFromName(TEXT("img_HelpBg")));;
}

void UUITreeViewItem_Help::NativeOnListItemObjectSet(UObject* pViewData)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(pViewData);

	m_pViewData = pViewData;

	const auto& pViewData_HelpCategory = Cast<UViewData_HelpCategory>(pViewData);
	if(pViewData_HelpCategory)
	{
		m_pCanvasHelpCategory->SetVisibility(ESlateVisibility::Visible);
		m_pCanvasHelp->SetVisibility(ESlateVisibility::Hidden);

		const auto& pHelpCategoryData = pViewData_HelpCategory->GetData();
		auto& cMgr = *Managers::LZ;
		m_pTxtHelpCategoryName->SetText(FText::FromString(cMgr[pHelpCategoryData->Name]));
		return;
	}

	const auto& pViewData_Help = Cast<UViewData_Help>(pViewData);
	if(pViewData_Help)
	{
		m_pCanvasHelpCategory->SetVisibility(ESlateVisibility::Hidden);
		m_pCanvasHelp->SetVisibility(ESlateVisibility::Visible);
		
		pViewData_Help->SetItem(this);

		const auto& pHelpData = pViewData_Help->GetData();
		auto& cMgr = *Managers::LZ;
		m_pTxtHelpName->SetText(FText::FromString(cMgr[pHelpData->Name]));

		SetSelected(pViewData_Help->GetSelected());
	}
}

void UUITreeViewItem_Help::SetSelected(const bool& bSelected)
{
	if(bSelected)
	{
		m_pImgHelpBgSelected->SetVisibility(ESlateVisibility::Visible);
		m_pImgHelpBgUnselected->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		m_pImgHelpBgSelected->SetVisibility(ESlateVisibility::Hidden);
		m_pImgHelpBgUnselected->SetVisibility(ESlateVisibility::Visible);
	}
}

bool UUITreeViewItem_Help::IsSame(UObject* pViewData)
{
	return m_pViewData == pViewData;
}
