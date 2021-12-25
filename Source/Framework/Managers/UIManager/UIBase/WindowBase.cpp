#include "WindowBase.h"

#include "UIModuleBase.h"
#include "Framework/Managers/Managers.h"

UWindowBase::UWindowBase(const FObjectInitializer& ObjectInitializer)
	: UImmortalWidget(ObjectInitializer)
{
	m_bPopup = false;
	m_bUseModalBlock = false;
	m_bUseMouse = true;
}

UWindowBase::~UWindowBase()
{
	if(m_pNode)
	{
		m_pNode->RemoveSelf();

		delete m_pNode;
		m_pNode = nullptr;
	}
}

void UWindowBase::BeginDestroy()
{
	Super::BeginDestroy();
}

TDoubleLinkedListEx<UWindowBase*>::TDoubleLinkedListExNode* UWindowBase::GetNode()
{
	if(m_pNode == nullptr)
		m_pNode = new TDoubleLinkedListEx<UWindowBase*>::TDoubleLinkedListExNode(this);

	return m_pNode;
}

bool UWindowBase::IsInStack() const
{
	return m_pNode && m_pNode->GetList();
}

const bool& UWindowBase::IsShowing() const
{
	return m_bShowing;
}

const bool& UWindowBase::IsPopup() const
{
	return m_bPopup;
}

const bool& UWindowBase::UseModalBlock() const
{
	return m_bUseModalBlock;
}

const int32& UWindowBase::GetZOrder() const
{
	return m_nZOrder;
}

bool UWindowBase::IsInLastStack()
{
	return Managers::UI->IsLastStackWindow(this);
}

void UWindowBase::SortingZOrder()
{
	int nZOrder = 0;
	auto node = m_llistModuleStack.GetHead();
	while (node)
	{
		node->GetValue()->SetZOrder(++nZOrder);
		node = node->GetNextNode();
	}	
}

void UWindowBase::OnWindowCreate()
{
	// 처음 인스턴스가 생성될 때 호출
	// 일반적인 NativeConstruct()는 쓰지않는다
	// AddToViewport때마다 호출되기 때문
}

void UWindowBase::OnWindowOpened(const bool& bOpen, const int32& nZOrder)
{
	// 윈도우가 화면에 띄워짐(이 메서드 이후 렌더링됨)
	// bOpen == true : 오픈 요청을 받고 띄워짐
	// bOpen == false : 스택 복구 등으로 가려졌다가 다시 나타남

	m_nZOrder = nZOrder;
	m_bShowing = true;

	if(!bOpen && m_mapModuleStackInfo.Num() > 0)
	{
		for(auto& pair : m_mapModuleStackInfo)
		{
			const auto& sInfo = pair.Value;
			UUIModuleBase* pModule = sInfo.Node->GetValue();
			pModule->OnUIModuleOpened(this);
			pModule->SetRoot(sInfo.pParent);
			pModule->SetPosition(sInfo.vLocation);
		}

		SortingZOrder();
	}
}

void UWindowBase::OnWindowClosed(const bool& bClosed)
{
	//	윈도우가 화면에 사라짐(이미 꺼져 있음)
	//	bOpen == true : 아예 스택에서 삭제
	//	bOpen == false : 스택에는 남아있음. 다른 Window에 의해 가려진 상태

	m_bShowing = false;
}

void UWindowBase::OnWindowLastStack()
{
	//	윈도우가 가장 마지막 스택으로 세팅됨
	//	OnWindowOpened 이후에 호출됨
	//	나보다 윗스택의 UI가 꺼져도 호출됨

	auto& pInputCtrl = Managers::UI->GetInputCtrl();
	if(pInputCtrl)
	{
		pInputCtrl->ClearKey(false);
		InitHotkey();
		pInputCtrl->SaveKey();
	}

	Managers::UI->GetInputCtrl()->SetMouseEnable(m_bUseMouse);
}

bool UWindowBase::IsLocked()
{
	return false;
}

void UWindowBase::Refresh()
{
	
}

bool UWindowBase::Close()
{
	return Managers::UI->CloseWindow(this);
}

void UWindowBase::InitHotkey()
{
	
}
