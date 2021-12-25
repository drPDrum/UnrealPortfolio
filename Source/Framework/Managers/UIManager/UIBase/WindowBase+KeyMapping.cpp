#include "WindowBase.h"
#include "Framework/Managers/Managers.h"

void UWindowBase::AddKey_PlayerCtrl()
{
	const auto& pUserInputCtrl = Managers::UI->GetInputCtrl();
	if (pUserInputCtrl)
		pUserInputCtrl->AddKey_PlayCtrl(EUserInputTypes::KeyboardAndMouse, false, false);
}

template <class UserClass>
void UWindowBase::AddKey(const FKey& key, UserClass* Object,
	typename FInputActionHandlerSignature::TUObjectMethodDelegate<UserClass>::FMethodPtr Func)
{
	const auto& pUserInputCtrl = Managers::UI->GetInputCtrl();
	if (pUserInputCtrl)
		pUserInputCtrl->AddKey(key, Object, Func, false);
}