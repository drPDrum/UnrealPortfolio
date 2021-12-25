#pragma once

#include "Framework/Managers/UIManager/UIBase/WindowBase.h"
#include "Window_GameMain.generated.h"

class UButton;

UCLASS()
class FRAMEWORK_API UWindow_GameMain : public UWindowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window GameMain", meta=(AllowPrivateAccess="true", DisplayName="Root Center"))
	UCanvasPanel*	m_pCanvasCenterRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window GameMain", meta=(AllowPrivateAccess="true", DisplayName="Root Left"))
	UCanvasPanel*	m_pCanvasLeftRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window GameMain", meta=(AllowPrivateAccess="true", DisplayName="Root Right"))
	UCanvasPanel*	m_pCanvasRightRoot;

	// 우하단 버튼들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window GameMain", meta=(AllowPrivateAccess="true", DisplayName="Btn Inventory"))
	UButton*		m_pBtnInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Window GameMain", meta=(AllowPrivateAccess="true", DisplayName="Btn Help"))
	UButton*		m_pBtnHelp;

public:
	UWindow_GameMain(const FObjectInitializer& ObjectInitializer);
	virtual void OnWindowCreate() override;

	virtual bool IsLocked() override;
protected:
	virtual void InitHotkey() override;

private:
	UFUNCTION()
	void OnClickInventory();	
	UFUNCTION()
	void OnClickHelp();

	void OnKeyDownShowMouse();
};