#pragma once

#include "Framework/Managers/UIManager/UIBase/UIModuleBase.h"
#include "UIModule_Inventory.generated.h"

class UListView;
class UTileView;
class CUserItemData;
class UCanvasPanel;
class UButton;

UENUM()
enum class EInvenViewTypes : uint8
{
	None,
	List,
	Tile,
};

UCLASS()
class UViewData_Inventory : public UObject
{
	GENERATED_BODY()

	CUserItemData* m_pUserItemData;
public:
	void SetData(CUserItemData* const pUserItemInfo);
	const CUserItemData* GetData();
};

UCLASS()
class FRAMEWORK_API UUIModule_Inventory : public UUIModuleBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UIModule Inventory", meta=(AllowPrivateAccess="true", DisplayName="List Panel"))
	UCanvasPanel* m_pPanelListView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UIModule Inventory", meta=(AllowPrivateAccess="true", DisplayName="Tile Panel"))
	UCanvasPanel* m_pPanelTileView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UIModule Inventory", meta=(AllowPrivateAccess="true", DisplayName="List"))
	UListView* m_pListView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UIModule Inventory", meta=(AllowPrivateAccess="true", DisplayName="Tile"))
	UTileView* m_pTileView;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UIModule Inventory", meta=(AllowPrivateAccess="true", DisplayName="Btn List"))
	UButton* m_pBtnListView;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UIModule Inventory", meta=(AllowPrivateAccess="true", DisplayName="Btn Tile"))
	UButton* m_pBtnTileView;

	EInvenViewTypes m_eVurViewType;

public:
	virtual void OnUIModuleOpened(UWindowBase* pWndParent) override;

	void OnClickListItem(UObject* pViewData);

	UFUNCTION()
	void OnClickChangeView();

	EInvenViewTypes ChangeNextViewType();

private:	
	void ChangeViewType(const EInvenViewTypes& eViewType);
};