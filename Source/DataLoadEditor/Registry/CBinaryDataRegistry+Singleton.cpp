#include "CBinaryDataRegistry.h"
#include "CDirectoryStatVisitor.h"
#include "../DataLoadEditorInfo.h"

CBinaryDataRegistry* CBinaryDataRegistry::m_instance = nullptr;

CBinaryDataRegistry* CBinaryDataRegistry::Instance(UBinaryDataFactory* pFactory)
{
	if(m_instance == nullptr)
	{
		m_instance = new CBinaryDataRegistry(pFactory);
		Init();
	}
	
	return m_instance;
}

CBinaryDataRegistry* CBinaryDataRegistry::Instance()
{	
	return m_instance;
}

void CBinaryDataRegistry::Init()
{
	if(m_instance == nullptr)
		return;

	if(m_instance->InitCheckFileDiff())
		m_instance->MakeTick();

	// IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();	
	// CDirectoryStatVisitor cVisitor;
	// bool bCheckFileValid = false;
	//
	// if(platformFile.IterateDirectoryStatRecursively(*DataLoadEditorInfo::PATH_RELATIVE_BINARY_ROOT, cVisitor))
	// {
	// 	if(!bCheckFileValid)
	// 		bCheckFileValid = true;
	// }
	//
	// if(bCheckFileValid)
	// {
	// 	if(m_instance->CheckFilesDiff())
	// 		m_instance->MakeTick();
	// }
}
