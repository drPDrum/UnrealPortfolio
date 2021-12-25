// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ImmortalWidget.h"
#include "RuntimeUtils/GenericEx/DoubleLinkedListEx.h"
#include "WindowBase.generated.h"

class UUIModuleBase;
class UCanvasPanel;

struct SModuleInfo
{
	TDoubleLinkedListEx<UUIModuleBase*>::TDoubleLinkedListExNode* Node;
	UCanvasPanel*	pParent;
	FVector2D		vLocation;

	SModuleInfo(TDoubleLinkedListEx<UUIModuleBase*>::TDoubleLinkedListExNode* node, UCanvasPanel* pParent, FVector2D vLocation);
};

UCLASS()
class FRAMEWORK_API UWindowBase : public UImmortalWidget
{
	GENERATED_BODY()

private: // For Window
	int32	m_nZOrder;
	bool	m_bShowing;
	TDoubleLinkedListEx<UWindowBase*>::TDoubleLinkedListExNode* m_pNode;

private: // For UIModule 	
	TDoubleLinkedListEx<UUIModuleBase*> m_llistModuleStack;
	TMap<UClass*, SModuleInfo> m_mapModuleStackInfo;
	
protected: // For Window Member Values
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WindowBase", meta=(AllowPrivateAccess="true", DisplayName="BGM"))
	USoundBase* m_pSound_BGM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WindowBase", meta=(AllowPrivateAccess="true", DisplayName="IsPopup"))
	bool m_bPopup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WindowBase", meta=(AllowPrivateAccess="true", DisplayName="Use Modal Block"))
	bool m_bUseModalBlock;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WindowBase", meta=(AllowPrivateAccess="true", DisplayName="Use Modal Block"))
	bool m_bUseMouse;
	
public: // For Window Misc
	UWindowBase(const FObjectInitializer& ObjectInitializer);
	virtual ~UWindowBase() override;
	virtual void BeginDestroy() override;

	TDoubleLinkedListEx<UWindowBase*>::TDoubleLinkedListExNode* GetNode();
	bool IsInStack() const;
	const bool& IsShowing() const;
	const bool& IsPopup() const;
	const bool& UseModalBlock() const;
	const int32& GetZOrder() const;
	bool IsInLastStack();

	void SortingZOrder();

public: // For Window Stack
	// 처음 인스턴스가 생성될 때 호출
	// 일반적인 NativeConstruct()는 쓰지않는다
	// AddToViewport때마다 호출되기 때문	
	virtual  void OnWindowCreate();
	
	// 윈도우가 화면에 띄워짐(이 메서드 이후 렌더링됨)
	// bOpen == true : 오픈 요청을 받고 띄워짐
	// bOpen == false : 스택 복구 등으로 가려졌다가 다시 나타남
	virtual void OnWindowOpened(const bool& bOpen, const int32& nZOrder);

	//	윈도우가 화면에 사라짐(이미 꺼져 있음)
	//	bOpen == true : 아예 스택에서 삭제
	//	bOpen == false : 스택에는 남아있음. 다른 Window에 의해 가려진 상태
	virtual void OnWindowClosed(const bool& bClosed);

	//	윈도우가 가장 마지막 스택으로 세팅됨
	//	OnWindowOpened 이후에 호출됨
	//	나보다 윗스택의 UI가 꺼져도 호출됨
	virtual void OnWindowLastStack();

	virtual bool IsLocked();
	virtual void Refresh();
	virtual bool Close();
protected:
	// 상속 후 마지막에 호출해 줄것
	virtual void InitHotkey();

public: // For UIModule
	template<typename T = UUIModuleBase>
	T* GetUIModule(const bool& bCreate);
	template<typename T = UUIModuleBase>
	bool IsOpenedUIModule();
	
	template<typename T = UUIModuleBase>
	T* OpenUIModule(UCanvasPanel* pCanvasPanel);
	template<typename T = UUIModuleBase>
	bool CloseUIModule();
	bool CloseUIModule(UUIModuleBase* pModule);
	void ClearUIModule();

	bool HasModule();
	bool CloseLastModule();

protected:
	static void AddKey_PlayerCtrl();
	template <class UserClass>
	static void AddKey(const FKey& key, UserClass* Object, typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr Func);
};


