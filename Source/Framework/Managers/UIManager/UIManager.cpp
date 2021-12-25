#include "UIManager.h"

#include "Framework/Managers/Managers.h"
#include "Window_SystemPopup/Window_SystemPopup.h"

constexpr int32 UIManager::ZORDER_WINDOW_START = 0;
constexpr int32 UIManager::ZORDER_WINDOW_INTERVAL = 100;
constexpr int32 UIManager::ZORDER_POPUP = 100000;

UIManager::UIManager()
{
	m_pWinPopup = nullptr;
	m_pModalBlock = nullptr;
	m_pUserInputCtrl = nullptr;
}

UIManager::~UIManager()
{
	if(m_pWinPopup)
	{
		Managers::GameInst->RemoveWidget(m_pWinPopup);
		m_pWinPopup = nullptr;
	}

	if(m_pModalBlock)
	{
		Managers::GameInst->RemoveWidget(m_pModalBlock);
		m_pModalBlock = nullptr;
	}

	m_llistWinStack.Empty(false);
}

bool UIManager::Init()
{
	auto pMgr = Managers::LZ;
	if(pMgr == nullptr)
	{
		LOG_ERROR(TEXT("Failed :: Should be Initialize after localization manager"));
		return false;
	}
	
	const auto& cMgr = *pMgr;
	m_strDefaultOk = cMgr["CODE_YES"];
	m_strDefaultNo = cMgr["CODE_NO"];
	m_strDefaultCancel = cMgr["CODE_CANCEL"];

	const auto& strPathModal = Managers::Data->ConstData->BP_MODAL;
	UClass* pModalClass = Managers::Asset->LoadBP<UUserWidget>(strPathModal);
	if(pModalClass)
	{
		m_pModalBlock = Cast<UUserWidget>(CreateWidget(Managers::GameInst, pModalClass, TEXT("UIModalBlock")));
		Managers::GameInst->AddWidget(m_pModalBlock);
	}
	else
	{
		LOG_ERROR(TEXT("Failed :: Not Found BP :: %s"), *strPathModal);
	}
	
	// UserInput 갱신
	m_pUserInputCtrl = Cast<AFrameworkInputController>(Managers::World->GetWorld()->GetFirstPlayerController());

	return true;
}

void UIManager::OnPostChangeLevel()
{
	// UserInput 갱신
	m_pUserInputCtrl = Cast<AFrameworkInputController>(Managers::World->GetWorld()->GetFirstPlayerController());

	// 로드가 끝날때까지 Input을 잠금,
	// 추후 AGameModeBase::OnStartGame에서 열어준다.
	m_pUserInputCtrl->SetActorTickEnabled(false);
}

AFrameworkInputController* const& UIManager::GetInputCtrl() const
{
	return m_pUserInputCtrl;
}

template <typename T>
T* UIManager::GetWindow(const bool& bCreate)
{
	const auto& uclass = T::StaticClass();

	if(m_mapWinInst.Contains(uclass))
		return Cast<T>(m_mapWinInst[uclass]);

	if(!bCreate)
		return nullptr;

	FString strWinName = T::StaticClass()->GetName();

	FString strRefPath = FString::Printf(TEXT("WidgetBlueprint'/Game/GameContents/UI/Windows/%s/%s.%s_C'"), *strWinName, *strWinName, *strWinName);
	auto bpClass = Managers::Asset->LoadBP<T>(strRefPath);
	if(bpClass == nullptr)
	{
		LOG_ERROR(TEXT("Failed :: Not Found BP :: %s"), *strRefPath);
		return nullptr;
	}

	auto pWin = Cast<T>(CreateWidget(Managers::GameInst, bpClass, *strWinName));
	if(pWin == nullptr)
		return nullptr;

	m_mapWinInst.Add(uclass, pWin);
	Managers::GameInst->AddWidget(pWin);

	pWin->OnWindowCreate();

	return pWin;
}

template <typename T>
T* UIManager::OpenWindow()
{
	auto pWin = GetWindow<T>(true);
	OpenWindow(pWin);

	return pWin;
}

void UIManager::OpenWindow(UWindowBase* pWin)
{
	if(pWin == nullptr)
		return;

	auto nodeLastWin = m_llistWinStack.GetTail();
	if(nodeLastWin && nodeLastWin->GetValue() == pWin)
		return;

	// 팝업이 아닐 경우 기존 Window 정리
	if(!pWin->IsPopup())
	{
		// ZOrder 초기화
		m_nZOrder = ZORDER_WINDOW_START;

		auto nodeWin = m_llistWinStack.GetTail();
		while(nodeWin)
		{
			auto nodePrevWin = nodeWin;
			auto pWinPrev = nodePrevWin->GetValue();

			nodeWin = nodeWin->GetPrevNode();

			// 이미 꺼져있다면 루프 멈춤
			if(!pWinPrev->IsShowing())
				break;

			// 뷰포트에서 제거
			pWinPrev->RemoveFromViewport();
			pWinPrev->OnWindowClosed(false);
		}
	}

	m_llistWinStack.AddTail(pWin->GetNode());

	int32 nCurZOrder = m_nZOrder;
	m_nZOrder += ZORDER_WINDOW_INTERVAL;

	pWin->AddToViewport(nCurZOrder);
	pWin->OnWindowOpened(true, nCurZOrder);
	pWin->OnWindowLastStack();

	RefreshModalBlock();
}

template <typename T>
bool UIManager::CloseWindow()
{
	auto pWinClose = GetWindow<T>(false);
	return CloseWindow(pWinClose);
}

bool UIManager::CloseWindow(UWindowBase* pWin)
{
	if(pWin == nullptr)
		return true;

	if(!pWin->IsInStack())
		return true;

	if(pWin->IsLocked())
		return false;

	// List에서 제거, Viewport에서 제거, Close Event
	pWin->GetNode()->RemoveSelf();
	pWin->RemoveFromViewport();
	pWin->OnWindowClosed(true);

	// Popup 이거나 남은 UI가 없으면 종료
	if (pWin->IsPopup() || m_llistWinStack.Num() == 0)
		return true;

	// 이전 Window 중 Popup이 아닌 것을 찾는다.
	auto nodeWin = m_llistWinStack.GetTail();
	while (nodeWin && !nodeWin->GetValue()->IsPopup() && nodeWin->GetPrevNode())
		nodeWin = nodeWin->GetPrevNode();

	// 해당 노드부터 차례대로 켜준다. (popup 복구)
	while (nodeWin)
	{
		auto pWnd_Prev = nodeWin->GetValue();
		nodeWin = nodeWin->GetNextNode();

		pWnd_Prev->AddToViewport(++m_nZOrder);
		pWnd_Prev->OnWindowOpened(false, m_nZOrder);
	}

	nodeWin = m_llistWinStack.GetTail();
	if (nodeWin)
		nodeWin->GetValue()->OnWindowLastStack();

	RefreshModalBlock();
	return true;
}

bool UIManager::CloseWindow(UWindowBase* pWnd, const bool& bDestroy)
{
	auto bResult = CloseWindow(pWnd);
	if (bDestroy && bResult)
		Managers::GameInst->RemoveWidget(pWnd);

	return bResult;
}

bool UIManager::CloseLastWindow()
{
	if (m_pWinPopup && m_pWinPopup->IsShowing())
	{
		m_pWinPopup->OnClickEscape();
		return true;
	}
	
	if (m_llistWinStack.Num() == 0)
		return true;
	
	auto nodeLast = m_llistWinStack.GetTail();
	auto pWin = nodeLast->GetValue();
	if (pWin->HasModule())
		return pWin->CloseLastModule();
	
	return CloseWindow(pWin);
}

void UIManager::ClearWindow(bool bDestroy)
{
	auto nodeWin = m_llistWinStack.GetTail();
	while (nodeWin)
	{
		auto nodeClose = nodeWin;
		auto pWinClose = nodeClose->GetValue();

		nodeWin = nodeWin->GetPrevNode();

		// 스택과 뷰포트에서 제거
		nodeClose->RemoveSelf();
		if (pWinClose->IsInViewport())
			pWinClose->RemoveFromViewport();
	}

	// 바인딩된 키 모두 제거
	if (m_pUserInputCtrl)
		m_pUserInputCtrl->ClearKey(true);

	// 삭제
	if (bDestroy)
	{
		for (auto& pair : m_mapWinInst)
			Managers::GameInst->RemoveWidget(pair.Value);

		m_mapWinInst.Empty();
	}

	if (m_pModalBlock->IsInViewport())
		m_pModalBlock->RemoveFromViewport();
}

bool UIManager::IsLastStackWindow(UWindowBase* pWin) const
{
	const auto& nodeWin = pWin->GetNode();
	return nodeWin->GetList() && !nodeWin->GetNextNode();
}

void UIManager::RefreshModalBlock() const
{
	// 일단 Viewport에서 제거
	if (m_pModalBlock->IsInViewport())
		m_pModalBlock->RemoveFromViewport();

	// 시스템 팝업이 활성화 중이면
	if(m_pWinPopup && m_pWinPopup->IsShowing())
	{
		m_pModalBlock->AddToViewport(m_pWinPopup->GetZOrder() - 1);
		return;
	}

	auto nodeWin = m_llistWinStack.GetTail();
	while (nodeWin)
	{
		const auto& pWin = nodeWin->GetValue();
		if (!pWin->IsShowing())
			break;
		
		if (pWin->UseModalBlock())
		{
			m_pModalBlock->AddToViewport(pWin->GetZOrder() - 1);
			break;
		}
		
		nodeWin = nodeWin->GetPrevNode();
	}
}