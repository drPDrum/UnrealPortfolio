#include "LoadingBase.h"
#include "Framework/Managers/Managers.h"

void ULoadingBase::Open()
{
	OnEndAnim_Open();
}

void ULoadingBase::Close()
{
	OnEndAnim_Close();
}

void ULoadingBase::OnEndAnim_Open() const
{
	Managers::World->OnEnd_LoadingAnim_Open();
}

void ULoadingBase::OnEndAnim_Close() const
{
	Managers::World->OnEnd_LoadingAnim_Close();
}