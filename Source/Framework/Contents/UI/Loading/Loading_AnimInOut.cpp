#include "Loading_AnimInOut.h"

void ULoading_AnimInOut::Open()
{
	PlayAnimation(GetAnimation("OpenAnim"));
}

void ULoading_AnimInOut::Close()
{
	PlayAnimation(GetAnimation("CloseAnim"));
}
