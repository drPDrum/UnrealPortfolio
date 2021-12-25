#include "GameMode_Intro.h"

#include "Framework/Contents/UI/Intro/Window_Intro.h"
#include "Framework/Managers/Managers.h"

AGameMode_Intro::AGameMode_Intro()
{
}

void AGameMode_Intro::OnInit()
{
	Managers::UI->OpenWindow<UWindow_Intro>();
	
	Super::OnInit();
}
