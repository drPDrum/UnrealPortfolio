#include "UIModule_Inventory.h"

void UViewData_Inventory::SetData(CUserItemData* const pUserItemInfo)
{
	m_pUserItemData = pUserItemInfo;	
}

const CUserItemData* UViewData_Inventory::GetData()
{
	return m_pUserItemData;
}
