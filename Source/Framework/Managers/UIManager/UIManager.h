// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>

#include "Framework/CommonInfo.h"
#include "Framework/Managers/IManager.h"
#include "Framework/UEBaseClass/FrameworkInputController.h"
#include "RuntimeUtils/GenericEx/DoubleLinkedListEx.h"

class UWindowBase;
class UUIModuleBase;
class ULoadingBase;
class UWindow_SystemPopup;

class FRAMEWORK_API UIManager : public IManagers
{
private: // For Window
	static const int32 ZORDER_WINDOW_START;
	static const int32 ZORDER_WINDOW_INTERVAL;
	static const int32 ZORDER_POPUP;
	
	int32								m_nZOrder = 0;
	TDoubleLinkedListEx<UWindowBase*>	m_llistWinStack;
	TMap<UClass*, UWindowBase*>			m_mapWinInst;

	UUserWidget*						m_pModalBlock;

private : // For Popup
	UWindow_SystemPopup*				m_pWinPopup;
	
	FString 							m_strDefaultOk;
	FString 							m_strDefaultNo;
	FString 							m_strDefaultCancel;

private :
	/*<UIModuleBase::StaticClass, refCount>*/
	TMap<UClass*, int>					m_mapModuleRefCount;
	TMap<UClass*, UUIModuleBase*>		m_mapModulelInst;
	AFrameworkInputController*			m_pUserInputCtrl;

public:
	UIManager();
	virtual ~UIManager() override;
public:
	bool Init() override;
	void OnPostChangeLevel();
	AFrameworkInputController* const& GetInputCtrl() const;

public: // For Window	
	template<typename T = UWindowBase>
   	T* GetWindow(const bool& bCreate);
	template<typename T = UWindowBase>
	T* OpenWindow();
	void OpenWindow(UWindowBase* pWin);
	template<typename T = UWindowBase>
	bool CloseWindow();
	bool CloseWindow(UWindowBase* pWin);
	bool CloseWindow(UWindowBase* pWnd, const bool& bDestroy);
	bool CloseLastWindow();
	void ClearWindow(bool bDestroy);
	bool IsLastStackWindow(UWindowBase* pWin) const;

	void RefreshModalBlock() const;

public : // For Loading
	ULoadingBase* OpenLoading(const FString& strLoadingName);
	void CloseLoading(ULoadingBase* pLoadingPage);

public : // For Popup
	void Popup(const FString& strTitle, const FString& strContents, const FString& strOk, std::function<void()> onOkCallback);
	void Popup(const FString& strTitle, const FString& strContents, const FString& strOk, std::function<void()> onOkCallback, const FString& strCancel, std::function<void()> onCancelCallback);
	void Popup(const FString& strTitle, const FString& strContents, const FString& strOk, std::function<void()> onOkCallback, const FString& strNo, std::function<void()> onNoCallback, const FString& strCancel, std::function<void()> onCancelCallback);
	void Popup(const FString& strTitle, const FString& strContents, std::function<void()> onOkCallback);
	void Popup(const FString& strTitle, const FString& strContents, std::function<void()> onOkCallback, std::function<void()> onCancelCallback);
	void Popup(const FString& strTitle, const FString& strContents, std::function<void()> onOkCallback, std::function<void()> onNoCallback, std::function<void()> onCancelCallback);

public: // For Module
	template<typename T = UUIModuleBase>
	T* GetUIModule(const bool& bCreate);
	void AddUIModuleRefCount(UClass* pModuleUClass);
	void SubUIModuleRefCount(UClass* pModuleUClass);
};


