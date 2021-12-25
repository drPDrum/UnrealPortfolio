#include "UIModule_Help.h"
#include "UITreeViewItem_Help.h"

void UViewData_Help::SetData(const CHelpData* const pData)
{
	m_pData = pData;
	m_bSelected = false;
}

const CHelpData* const& UViewData_Help::GetData() const
{
	return m_pData;
}

void UViewData_Help::SetItem(UUITreeViewItem_Help* pTreeViewItem)
{
	m_pTreeViewItem = pTreeViewItem;
}

void UViewData_Help::SetSelected(const bool& bSelected)
{
	m_bSelected = bSelected;

	if(m_pTreeViewItem && m_pTreeViewItem->IsSame(this))
		m_pTreeViewItem->SetSelected(bSelected);
}

const bool& UViewData_Help::GetSelected()
{
	return m_bSelected;
}