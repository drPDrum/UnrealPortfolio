#include "UIModuleBase.h"
#include "WindowBase.h"
#include "Framework/Managers/Managers.h"

SModuleInfo::SModuleInfo(TDoubleLinkedListEx<UUIModuleBase*>::TDoubleLinkedListExNode* node, UCanvasPanel* pParent, FVector2D vLocation)
	: Node(node), pParent(pParent), vLocation(vLocation) { }

template <typename T>
T* UWindowBase::GetUIModule(const bool& bCreate)
{
	return Managers::UI->GetUIModule<T>(bCreate);
}

template <typename T>
bool UWindowBase::IsOpenedUIModule()
{
	return m_mapModuleStackInfo.Contains(T::StaticClass());
}

template <typename T>
T* UWindowBase::OpenUIModule(UCanvasPanel* pCanvasPanel)
{
	if(!pCanvasPanel)
	{
		LOG_ERROR(TEXT("Failed :: %s :: Canvas Panel is NULL"), *(T::StaticClass()->GetName()));
		return nullptr;
	}

	// Get Instance
	auto pModule = GetUIModule<T>(true);
	if(!pModule)
		return nullptr;

	const auto& uclass = T::StaticClass();

	// Add stack
	if(m_mapModuleStackInfo.Contains(uclass))
	{
		// 이미 스택에 있으면, 순서만 바꾼다.		
		auto& sInfo = m_mapModuleStackInfo[uclass];
		m_llistModuleStack.AddTail(sInfo.Node);

		if(sInfo.pParent != pCanvasPanel)
		{
			sInfo.pParent = pCanvasPanel;
			sInfo.vLocation = FVector2D::ZeroVector;
			pModule->SetRoot(pCanvasPanel);
			pModule->SetPosition(FVector2D::ZeroVector);			
		}
	}
	else
	{
		// 스택에 없으면 새로 노드 만들고 추가.
		auto node = new TDoubleLinkedListEx<UUIModuleBase*>::TDoubleLinkedListExNode(pModule);
		m_llistModuleStack.AddTail(node);
		m_mapModuleStackInfo.Add(uclass, SModuleInfo(node, pCanvasPanel, FVector2D::ZeroVector));

		// Add ref count, open
		Managers::UI->AddUIModuleRefCount(uclass);
		pModule->SetRoot(pCanvasPanel);
		pModule->SetPosition(FVector2D::ZeroVector);
		pModule->OnUIModuleOpened(this);
	}

	// ZOrder Sort
	SortingZOrder();

	// 마우스를 사용하지 않는 UI에서 모듈을 키면 커서를 켜준다.
	if (!m_bUseMouse && m_llistModuleStack.Num() > 0)
		Managers::UI->GetInputCtrl()->SetMouseEnable(true);

	return pModule;
}

template <typename T>
bool UWindowBase::CloseUIModule()
{
	return CloseUIModule(GetUIModule<T>(false));
}

bool UWindowBase::CloseUIModule(UUIModuleBase* pModule)
{
	if(!pModule)
		return true;

	const auto& uclass = pModule->GetStaticClass();

	if(!m_mapModuleStackInfo.Contains(uclass))
		return true;

	// 잠김 체크
	if(pModule->IsLocked())
		return false;

	auto node = m_mapModuleStackInfo[uclass].Node;

	// remove stack, info
	node->RemoveSelf();
	m_mapModuleStackInfo.Remove(uclass);
	delete node;

	pModule->OnUIModuleClosed();

	// sub ref count
	Managers::UI->SubUIModuleRefCount(uclass);

	// 마우스를 사용하지 않는 UI에서 모듈을 껏을때 0개면 마우스도 꺼준다.
	if (!m_bUseMouse && m_llistModuleStack.Num() == 0)
		Managers::UI->GetInputCtrl()->SetMouseEnable(false);

	return true;
}

void UWindowBase::ClearUIModule()
{
	auto node = m_llistModuleStack.GetHead();
	while(node)
	{
		auto pModule = node->GetValue();
		if(Managers::IsInit())
			Managers::UI->SubUIModuleRefCount(pModule->GetStaticClass());

		node = node->GetNextNode();
	}

	m_mapModuleStackInfo.Empty();
	m_llistModuleStack.Empty(true);
}

bool UWindowBase::HasModule()
{
	return m_llistModuleStack.Num() > 0;
}

bool UWindowBase::CloseLastModule()
{
	auto node = m_llistModuleStack.GetTail();
	if(node)
		return CloseUIModule(node->GetValue());

	return true;
}