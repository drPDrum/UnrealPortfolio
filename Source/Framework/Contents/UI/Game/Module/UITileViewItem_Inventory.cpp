#include "UITileViewItem_Inventory.h"

#include "UIModule_Inventory.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Framework/Managers/Managers.h"
#include "Framework/UserInfo/UserData/CUserItemData.h"

void UUITileViewItem_Inventory::NativeConstruct()
{
	Super::NativeConstruct();
	
	m_pImgIcon = Cast<UImage>(GetWidgetFromName(TEXT("Cimg_Icon")));
	m_pTxtCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("Ctxt_Count")));
}

void UUITileViewItem_Inventory::NativeOnListItemObjectSet(UObject* pViewData)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(pViewData);

	const auto& pViewDataInven = Cast<UViewData_Inventory>(pViewData);
	const auto& pUserData = pViewDataInven->GetData();
	const auto& pItemData = pUserData->GetData();

	// 스프라이트 세팅
	const auto& refPath = FString::Printf(TEXT("Texture2D'/Game/GameContents/UITexture/Icon_Item/%s.%s'"), *pItemData->ImgNameIcon, *pItemData->ImgNameIcon);
	m_pImgIcon->SetBrushFromTexture(Managers::Asset->LoadObj<UTexture2D>(*refPath));

	// 갯수 세팅
	const auto& nCount = pUserData->GetStackCount();
	if (nCount > 1)
	{
		m_pTxtCount->SetVisibility(ESlateVisibility::Visible);
		m_pTxtCount->SetText(FText::FromString(FString::Printf(TEXT("x %d"), nCount)));
	}
	else
	{
		m_pTxtCount->SetVisibility(ESlateVisibility::Hidden);
	}
}
