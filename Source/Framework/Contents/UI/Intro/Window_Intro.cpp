#include "Window_Intro.h"

#include "Components/Button.h"
#include "Framework/Contents/UI/Common/Module/UIModule_Help.h"
#include "Framework/Managers/Managers.h"

UWindow_Intro::UWindow_Intro(const FObjectInitializer& ObjectInitializer)
	: UWindowBase(ObjectInitializer)
{
	bIsFocusable = true;
	m_bUseMouse = true;
}

void UWindow_Intro::OnWindowCreate()
{
	Super::OnWindowCreate();

	//변수 이름과 BP상의 이름 동일하게 한 경우. 상속받은 bp 컴파일 시 인식한다.	
	BtnStart->OnClicked.AddDynamic(this, &UWindow_Intro::OnClick_StartGame);
}

void UWindow_Intro::InitHotkey()
{
	AddKey_PlayerCtrl();
	AddKey(EKeys::One, this, &UWindow_Intro::OnClick_UIModule_Help);
}

void UWindow_Intro::OnClick_StartGame()
{
	LOG(TEXT("TODO : OnClick StartGame"));
	Managers::World->LoadWorld(EWorldId::Game,
		Managers::Data->ConstData->LOADING_COMMON);
}

void UWindow_Intro::OnClick_UIModule_Help()
{
	if(IsOpenedUIModule<UUIModule_Help>())
		CloseUIModule<UUIModule_Help>();
	else
		OpenUIModule<UUIModule_Help>(Canvas_CenterRoot);
}
