#include "ActionEventRuntime_MoveInputE.h"
#include "../../BrainControl/BrainController.h"

UActionEventRuntime_MoveInputE::UActionEventRuntime_MoveInputE()
{
}

UActionEventRuntime_MoveInputE::~UActionEventRuntime_MoveInputE()
{
}

bool UActionEventRuntime_MoveInputE::OnStart(float ElapsedTime)
{
	// 속도 세팅
	m_pOwner->GetCharacterMovement()->MaxWalkSpeed = m_pData->m_fMoveSpdPerSec;

	// 방향 업데이트
	m_vDstWorldDir = m_pOwner->GetBrainCtrl()->GetAxisWorldDirV();

	return false;
}

bool UActionEventRuntime_MoveInputE::OnTick(float ElapsedTime, float DeltaTime)
{
	// 이동
	m_pOwner->AddMovementInput(m_vDstWorldDir, 1.f, true);

	return false;
}
