#include "UIModule_Help.h"

void UViewData_HelpCategory::SetData(const CHelpCategoryData* const pData, const TArray<const CHelpData*>& arrHelpData)
{
	m_pData = pData;

	for(auto& pHelpData : arrHelpData)
	{
		auto pViewData_Help = NewObject<UViewData_Help>(this);
		pViewData_Help->SetData(pHelpData);
		m_arrHelpDatas.Add(pViewData_Help);
	}
}

const CHelpCategoryData* const& UViewData_HelpCategory::GetData() const
{
	return m_pData;
}

TArray<UObject*>& UViewData_HelpCategory::GetChildrenData()
{
	return m_arrHelpDatas;
}