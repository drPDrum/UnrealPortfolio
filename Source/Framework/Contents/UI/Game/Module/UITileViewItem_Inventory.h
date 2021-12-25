#pragma once
#include "Framework/Managers/UIManager/UIBase/ImmortalWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UITileViewItem_Inventory.generated.h"

class UImage;
class UTextBlock;
class UserItemInfo;


UCLASS()
class FRAMEWORK_API UUITileViewItem_Inventory : public UImmortalWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UIScrollItem Inventory", meta=(AllowPrivateAccess="true", DisplayName="Img Icon"))
	UImage* m_pImgIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UIScrollItem Inventory", meta=(AllowPrivateAccess="true", DisplayName="Txt Count"))
	UTextBlock* m_pTxtCount;

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* pViewData) override;
};