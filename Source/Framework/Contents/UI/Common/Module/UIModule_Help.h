#pragma once
#include "Framework/Managers/UIManager/UIBase/UIModuleBase.h"
#include "UIModule_Help.generated.h"

class UTreeView;
class UMultiLineEditableText;
class UUITreeViewItem_Help;
class CHelpCategoryData;
class CHelpData;

UCLASS()
class UViewData_Help : public UObject
{
	GENERATED_BODY()

	const CHelpData*		m_pData;
	bool					m_bSelected;
	UUITreeViewItem_Help*	m_pTreeViewItem;

public:
	void SetData(const CHelpData* const pData);
	const CHelpData* const& GetData() const;

	void SetItem(UUITreeViewItem_Help* pTreeViewItem);
	void SetSelected(const bool& bSelected);
	const bool&  GetSelected();
};

UCLASS()
class UViewData_HelpCategory : public UObject
{
	GENERATED_BODY()

	const CHelpCategoryData*	m_pData;
	UPROPERTY()
	TArray<UObject*>			m_arrHelpDatas;

public:
	void SetData(const CHelpCategoryData* const pData, const TArray<const CHelpData*>& arrHelpData);
	const CHelpCategoryData* const& GetData() const;
	TArray<UObject*>& GetChildrenData();
};

UCLASS()
class FRAMEWORK_API UUIModule_Help : public UUIModuleBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UIModule Help", meta=(AllowPrivateAccess="true", DisplayName="Tile"))
	UTreeView*					m_pTreeView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UIModule Help", meta=(AllowPrivateAccess="true", DisplayName="Contents"))
	UMultiLineEditableText*		m_pTxtHelpContents;
	
	UPROPERTY()
	UViewData_Help*				m_pViewData_Help_Selected;
	
public:
	virtual void OnUIModuleCreate(UClass* pBaseUClass) override;

	UFUNCTION()
	void OnClickItem(UObject* pViewData);
	
	UFUNCTION()
	void GetItemChildren(UObject* pViewData, TArray<UObject*>& arrChildrenViewData);
};