#include "Window_SystemPopup.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/MultiLineEditableText.h"
#include "Components/TextBlock.h"
#include "Framework/Managers/Managers.h"

void UWindow_SystemPopup::OnWindowCreate()
{
	Super::OnWindowCreate();

	m_pCanvasLastAttached = nullptr;
	m_pCanvasRoot = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasRoot")));

	m_pTxtTitle = Cast<UTextBlock>(GetWidgetFromName(TEXT("CTxtTitle")));
	m_pTxtContents = Cast<UMultiLineEditableText>(GetWidgetFromName(TEXT("CTxtContents")));

	// Button Roots
	m_pCanvasBtnSetRoot_01 = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasBtnSet_01")));
	m_pCanvasBtnSetRoot_02 = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasBtnSet_02")));
	m_pCanvasBtnSetRoot_03 = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasBtnSet_03")));

	// Buttons
	m_pBtnOk_01 = Cast<UButton>(GetWidgetFromName(TEXT("BtnOk_01")));
	m_pBtnOk_02 = Cast<UButton>(GetWidgetFromName(TEXT("BtnOk_02")));
	m_pBtnOk_03 = Cast<UButton>(GetWidgetFromName(TEXT("BtnOk_03")));

	m_pBtnCancel_02 = Cast<UButton>(GetWidgetFromName(TEXT("BtnCancel_02")));
	m_pBtnCancel_03 = Cast<UButton>(GetWidgetFromName(TEXT("BtnCancel_03")));

	m_pBtnNo_03 = Cast<UButton>(GetWidgetFromName(TEXT("BtnNo_03")));

	// Set Button Event
	m_pBtnOk_01->OnClicked.AddDynamic(this, &UWindow_SystemPopup::OnClickOk);
	m_pBtnOk_02->OnClicked.AddDynamic(this, &UWindow_SystemPopup::OnClickOk);
	m_pBtnOk_03->OnClicked.AddDynamic(this, &UWindow_SystemPopup::OnClickOk);

	m_pBtnCancel_02->OnClicked.AddDynamic(this, &UWindow_SystemPopup::OnClickCancel);
	m_pBtnCancel_03->OnClicked.AddDynamic(this, &UWindow_SystemPopup::OnClickCancel);

	m_pBtnNo_03->OnClicked.AddDynamic(this, &UWindow_SystemPopup::OnClickNo);

	// Button Text
	m_pTxtOk_01 = Cast<UTextBlock>(GetWidgetFromName(TEXT("CTxtOk_01")));
	m_pTxtOk_02 = Cast<UTextBlock>(GetWidgetFromName(TEXT("CTxtOk_02")));
	m_pTxtOk_03 = Cast<UTextBlock>(GetWidgetFromName(TEXT("CTxtOk_03")));

	m_pTxtCancel_02 = Cast<UTextBlock>(GetWidgetFromName(TEXT("CTxtCancel_02")));
	m_pTxtCancel_03 = Cast<UTextBlock>(GetWidgetFromName(TEXT("CTxtCancel_03")));

	m_pTxtNo_03 = Cast<UTextBlock>(GetWidgetFromName(TEXT("CTxtNo_03")));

	// 모든 BtnSet을 화면에서 지운다.
	m_pCanvasBtnSetRoot_01->SetVisibility(ESlateVisibility::Hidden);
	m_pCanvasBtnSetRoot_02->SetVisibility(ESlateVisibility::Hidden);
	m_pCanvasBtnSetRoot_03->SetVisibility(ESlateVisibility::Hidden);
}

void UWindow_SystemPopup::BeginDestroy()
{
	Super::BeginDestroy();
}

void UWindow_SystemPopup::Init(const FString& strTitle, const FString& strContents, const FString& strOk,
	std::function<void()> onOkCallback)
{
	InitCommon(strTitle, strContents, 1, onOkCallback, nullptr, nullptr);
	ChangeRoot(m_pCanvasBtnSetRoot_01);

	m_pTxtOk_01->SetText(FText::FromString(strOk));
}

void UWindow_SystemPopup::Init(const FString& strTitle, const FString& strContents, const FString& strOk,
	std::function<void()> onOkCallback, const FString& strCancel, std::function<void()> onCancelCallback)
{
	InitCommon(strTitle, strContents, 2, onOkCallback, nullptr, onCancelCallback);
	ChangeRoot(m_pCanvasBtnSetRoot_02);

	m_pTxtOk_02->SetText(FText::FromString(strOk));
	m_pTxtCancel_02->SetText(FText::FromString(strCancel));
}

void UWindow_SystemPopup::Init(const FString& strTitle, const FString& strContents, const FString& strOk,
	std::function<void()> onOkCallback, const FString& strNo, std::function<void()> onNoCallback,
	const FString& strCancel, std::function<void()> onCancelCallback)
{
	InitCommon(strTitle, strContents, 3, onOkCallback, onNoCallback, onCancelCallback);
	ChangeRoot(m_pCanvasBtnSetRoot_03);
    
    m_pTxtOk_03->SetText(FText::FromString(strOk));
	m_pTxtNo_03->SetText(FText::FromString(strNo));
    m_pTxtCancel_03->SetText(FText::FromString(strCancel));	
}

void UWindow_SystemPopup::OnClickOk()
{
	CallAndClear(m_onOkCallback);
}

void UWindow_SystemPopup::OnClickNo()
{
	CallAndClear(m_onNoCallback);
}

void UWindow_SystemPopup::OnClickCancel()
{
	CallAndClear(m_onCancelCallback);
}

void UWindow_SystemPopup::OnClickEscape()
{
	switch (m_nBtnType)
	{
		case 1:
			OnClickOk();
			break;
		default:
			OnClickCancel();
			break;
	}
}

bool UWindow_SystemPopup::Close()
{
	RemoveFromViewport();
	OnWindowClosed(false);
	Managers::UI->RefreshModalBlock();

	return true;
}

void UWindow_SystemPopup::CallAndClear(std::function<void()> call)
{
	Close();

	m_onOkCallback = nullptr;
	m_onNoCallback = nullptr;
	m_onCancelCallback = nullptr;

	if(call)
		call();
}

void UWindow_SystemPopup::ChangeRoot(UCanvasPanel*& pCanvasBtnSetRoot)
{
	if(m_pCanvasLastAttached)
	{
		if(m_pCanvasLastAttached == pCanvasBtnSetRoot)
			return;

		m_pCanvasLastAttached->SetVisibility(ESlateVisibility::Hidden);
	}

	m_pCanvasLastAttached = pCanvasBtnSetRoot;
	m_pCanvasLastAttached->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UWindow_SystemPopup::InitCommon(const FString& strTitle, const FString& strContents, const int& nBtnType,
	std::function<void()> onOkCallback, std::function<void()> onNoCallback, std::function<void()> onCancelCallback)
{
	m_onOkCallback = onOkCallback;
	m_onNoCallback = onNoCallback;
	m_onCancelCallback = onCancelCallback;

	m_nBtnType = nBtnType;
	m_pTxtTitle->SetText(FText::FromString(strTitle));
	m_pTxtContents->SetText(FText::FromString(strContents));
}
