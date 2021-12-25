#include "GameMode_ManagerTest.h"
#include "Framework/Managers/Managers.h"

void AGameMode_ManagerTest::OnTickDebugInputHandler()
{
	auto& pInput = Managers::UI->GetInputCtrl();
	if(pInput->WasInputKeyJustPressed(EKeys::Two))
	{
		TestCodePopup(1);
	}
	else if(pInput->WasInputKeyJustPressed(EKeys::Three))
	{
		TestCodePopup(2);
	}
	else if(pInput->WasInputKeyJustPressed(EKeys::Four))
	{
		TestCodePopup(3);
	}
	else if(pInput->WasInputKeyJustPressed(EKeys::Five))
	{
		TestCodePopup(1, false);
	}
	else if(pInput->WasInputKeyJustPressed(EKeys::Six))
	{
		TestCodePopup(2, false);
	}
	else if(pInput->WasInputKeyJustPressed(EKeys::Seven))
	{
		TestCodePopup(3, false);
	}
}
