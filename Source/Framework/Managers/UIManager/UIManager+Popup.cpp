#include "UIManager.h"
#include "Window_SystemPopup/Window_SystemPopup.h"

void UIManager::Popup(const FString& strTitle, const FString& strContents, const FString& strOk,
                      std::function<void()> onOkCallback)
{
	if(!m_pWinPopup)
		m_pWinPopup = GetWindow<UWindow_SystemPopup>(true);

	m_pWinPopup->Init(strTitle, strContents, strOk, onOkCallback);

	if(!m_pWinPopup->IsInViewport())
		m_pWinPopup->AddToViewport(ZORDER_POPUP);

	m_pWinPopup->OnWindowOpened(true, ZORDER_POPUP);
	RefreshModalBlock();
}

void UIManager::Popup(const FString& strTitle, const FString& strContents, const FString& strOk,
	std::function<void()> onOkCallback, const FString& strCancel, std::function<void()> onCancelCallback)
{
	if(!m_pWinPopup)
		m_pWinPopup = GetWindow<UWindow_SystemPopup>(true);

	m_pWinPopup->Init(strTitle, strContents, strOk, onOkCallback, strCancel, onCancelCallback);

	if(!m_pWinPopup->IsInViewport())
		m_pWinPopup->AddToViewport(ZORDER_POPUP);

	m_pWinPopup->OnWindowOpened(true, ZORDER_POPUP);
	RefreshModalBlock();
}

void UIManager::Popup(const FString& strTitle, const FString& strContents, const FString& strOk,
	std::function<void()> onOkCallback, const FString& strNo, std::function<void()> onNoCallback,
	const FString& strCancel, std::function<void()> onCancelCallback)
{
	if(!m_pWinPopup)
		m_pWinPopup = GetWindow<UWindow_SystemPopup>(true);

	m_pWinPopup->Init(strTitle, strContents, strOk, onOkCallback, strNo, onNoCallback, strCancel, onCancelCallback);

	if(!m_pWinPopup->IsInViewport())
		m_pWinPopup->AddToViewport(ZORDER_POPUP);

	m_pWinPopup->OnWindowOpened(true, ZORDER_POPUP);
	RefreshModalBlock();
}

void UIManager::Popup(const FString& strTitle, const FString& strContents, std::function<void()> onOkCallback)
{
	Popup(strTitle, strContents, m_strDefaultOk, onOkCallback);
}

void UIManager::Popup(const FString& strTitle, const FString& strContents, std::function<void()> onOkCallback,
	std::function<void()> onCancelCallback)
{
	Popup(strTitle, strContents, m_strDefaultOk, onOkCallback,
		m_strDefaultCancel, onCancelCallback);	
}

void UIManager::Popup(const FString& strTitle, const FString& strContents, std::function<void()> onOkCallback,
	std::function<void()> onNoCallback, std::function<void()> onCancelCallback)
{
	Popup(strTitle, strContents, m_strDefaultOk, onOkCallback,
		m_strDefaultNo, onNoCallback,
		m_strDefaultCancel, onCancelCallback);
}