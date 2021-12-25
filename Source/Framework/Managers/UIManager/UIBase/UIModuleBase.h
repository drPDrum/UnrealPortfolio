// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ImmortalWidget.h"
#include "UIModuleBase.generated.h"

class UWindowBase;
class UCanvasPanel;
class UCanvasPanelSlot;

UCLASS()
class FRAMEWORK_API UUIModuleBase : public UImmortalWidget
{
	GENERATED_BODY()

private:
	UCanvasPanelSlot*	m_pCanvasSlot;
	UWindowBase*		m_pWinParent;
	UClass*				m_pBaseUClass;

public:
	UUIModuleBase(const FObjectInitializer& ObjectInitializer);
	virtual ~UUIModuleBase() override;

public:
	int32 GetZOrder() const;
	virtual void SetZOrder(const int32 zOrder);
	
public:
	// 블루프린트의 UClass가 아닌 베이스 클래스의 UClass를 받아온다.
	virtual void OnUIModuleCreate(UClass* pBaseUClass); 
	virtual void OnUIModuleOpened(UWindowBase* pWinParent);
	virtual void OnUIModuleClosed();
	
public:
	virtual bool IsLocked();
	virtual void Refresh();
	virtual bool Close();

	void SetRoot(UCanvasPanel* const& pRootPanel);
	void SetPosition(const FVector2D& vPos);
	FVector2D GetPosition() const;

	const UCanvasPanelSlot* GetCanvasSlot() const;
	const UWindowBase* GetParentWindow() const;
	UClass* GetStaticClass() const;
};