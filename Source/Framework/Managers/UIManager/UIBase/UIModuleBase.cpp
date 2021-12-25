#include "UIModuleBase.h"

#include "WindowBase.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UUIModuleBase::UUIModuleBase(const FObjectInitializer& ObjectInitializer)
	: UImmortalWidget(ObjectInitializer)
{
	m_pWinParent = nullptr;
	m_pCanvasSlot = nullptr;

	// 모듈은 기본적으로 포커싱 되지 않도록 한다.
	bIsFocusable = false;
}

UUIModuleBase::~UUIModuleBase()
{
	m_pWinParent = nullptr;
	m_pCanvasSlot = nullptr;
}

int32 UUIModuleBase::GetZOrder() const
{
	if(m_pCanvasSlot)
		return m_pCanvasSlot->GetZOrder();

	return -1;
}

void UUIModuleBase::SetZOrder(const int32 zOrder)
{
	if(m_pCanvasSlot)
		m_pCanvasSlot->SetZOrder(zOrder);
}

void UUIModuleBase::OnUIModuleCreate(UClass* pBaseUClass)
{
	m_pBaseUClass = pBaseUClass;
}

void UUIModuleBase::OnUIModuleOpened(UWindowBase* pWinParent)
{
	if(m_pWinParent == nullptr)
		return;

	m_pWinParent = pWinParent;
}

void UUIModuleBase::OnUIModuleClosed()
{
	RemoveFromParent();

	m_pWinParent = nullptr;
	m_pCanvasSlot = nullptr;
}

bool UUIModuleBase::IsLocked()
{
	return false;
}

void UUIModuleBase::Refresh()
{
	
}

bool UUIModuleBase::Close()
{
	if(m_pWinParent)
		return m_pWinParent->CloseUIModule(this);

	return false;
}

void UUIModuleBase::SetRoot(UCanvasPanel* const& pRootPanel)
{
	if(IsValid(m_pCanvasSlot))
	{
		if(m_pCanvasSlot->Parent == pRootPanel)
			return;

		RemoveFromParent();
	}

	m_pCanvasSlot = pRootPanel->AddChildToCanvas(this);

	// 기본 사이즈가 이상한 값으로 잡힌다. Zero로 세팅
	m_pCanvasSlot->SetSize(FVector2D::ZeroVector);
}

void UUIModuleBase::SetPosition(const FVector2D& vPos)
{
	if(m_pCanvasSlot)
		m_pCanvasSlot->SetPosition(vPos);
}

FVector2D UUIModuleBase::GetPosition() const
{
	if(m_pCanvasSlot)
		return m_pCanvasSlot->GetPosition();

	return FVector2D::ZeroVector;
}

const UCanvasPanelSlot* UUIModuleBase::GetCanvasSlot() const
{
	return m_pCanvasSlot;
}

const UWindowBase* UUIModuleBase::GetParentWindow() const
{
	return m_pWinParent;
}

UClass* UUIModuleBase::GetStaticClass() const
{
	return m_pBaseUClass;
}
