#pragma once

#include <functional>
#include "Framework/Managers/UIManager/UIBase/WindowBase.h"
#include "Window_SystemPopup.generated.h"

class UCanvasPanel;
class UButton;
class UTextBlock;
class UMultiLineEditableText;

UCLASS()
class FRAMEWORK_API UWindow_SystemPopup : public UWindowBase
{
	GENERATED_BODY()
	
private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCanvasPanel*			m_pCanvasRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*				m_pTxtTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMultiLineEditableText* m_pTxtContents;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCanvasPanel*			m_pCanvasBtnSetRoot_01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCanvasPanel*			m_pCanvasBtnSetRoot_02;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCanvasPanel*			m_pCanvasBtnSetRoot_03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton*				m_pBtnOk_01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton*				m_pBtnOk_02;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton*				m_pBtnOk_03;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton*				m_pBtnCancel_02;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton*				m_pBtnCancel_03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UButton*				m_pBtnNo_03;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*				m_pTxtOk_01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*				m_pTxtOk_02;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*				m_pTxtOk_03;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*				m_pTxtCancel_02;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*				m_pTxtCancel_03;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UTextBlock*				m_pTxtNo_03;

	UPROPERTY()
	UCanvasPanel*			m_pCanvasLastAttached;

	std::function<void()>	m_onOkCallback;
	std::function<void()>	m_onNoCallback;
	std::function<void()>	m_onCancelCallback;
	int						m_nBtnType;

public:
	virtual void OnWindowCreate() override;
	virtual void BeginDestroy() override;
	
	void Init(const FString& strTitle, const FString& strContents, const FString& strOk, std::function<void()> onOkCallback);
	void Init(const FString& strTitle, const FString& strContents, const FString& strOk, std::function<void()> onOkCallback,
		const FString& strCancel, std::function<void()> onCancelCallback);
	void Init(const FString& strTitle, const FString& strContents, const FString& strOk, std::function<void()> onOkCallback,
		const FString& strNo, std::function<void()> onNoCallback, const FString& strCancel, std::function<void()> onCancelCallback);

	UFUNCTION(BlueprintCallable)
	void OnClickOk();
	UFUNCTION(BlueprintCallable)
	void OnClickNo();
	UFUNCTION(BlueprintCallable)
	void OnClickCancel();

	void OnClickEscape();

	virtual bool Close() override;

private:
	void CallAndClear(std::function<void()> call);
	void ChangeRoot(UCanvasPanel*& pCanvasBtnSetRoot);
	void InitCommon(const FString& strTitle, const FString& strContents, const int& nBtnType, std::function<void()> onOkCallback, 
		std::function<void()> onNoCallback, std::function<void()> onCancelCallback);
};