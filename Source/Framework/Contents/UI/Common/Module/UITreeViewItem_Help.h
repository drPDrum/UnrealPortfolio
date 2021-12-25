#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "Framework/Managers/UIManager/UIBase/ImmortalWidget.h"
#include "UITreeViewItem_Help.generated.h"

class UTextBlock;
class UCanvasPanel;
class UImage;

UCLASS()
class FRAMEWORK_API UUITreeViewItem_Help : public UImmortalWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UITileViewItem Help", meta=(AllowPrivateAccess="true"))
	UCanvasPanel*	m_pCanvasHelpCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UITileViewItem Help", meta=(AllowPrivateAccess="true"))
	UCanvasPanel*	m_pCanvasHelp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UITileViewItem Help", meta=(AllowPrivateAccess="true"))
	UTextBlock*		m_pTxtHelpCategoryName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UITileViewItem Help", meta=(AllowPrivateAccess="true"))
	UTextBlock*		m_pTxtHelpName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UITileViewItem Help", meta=(AllowPrivateAccess="true"))
	UImage*			m_pImgHelpBgSelected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UITileViewItem Help", meta=(AllowPrivateAccess="true"))
	UImage*			m_pImgHelpBgUnselected;

	UPROPERTY()
	UObject*		m_pViewData;

public:
	virtual void NativeConstruct() override;
	virtual void NativeOnListItemObjectSet(UObject* pViewData) override;
	void SetSelected(const bool& bSelected);
	bool IsSame(UObject* pViewData);
};