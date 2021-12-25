#include "ActionEventRuntime_MoveNavE.h"
#include "../../BrainControl/BrainController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UActionEventRuntime_MoveNavE::UActionEventRuntime_MoveNavE()
{
}

UActionEventRuntime_MoveNavE::~UActionEventRuntime_MoveNavE()
{
}

bool UActionEventRuntime_MoveNavE::OnStart(float ElapsedTime)
{
	// 속도 세팅
	m_pOwner->GetCharacterMovement()->MaxWalkSpeed = m_pData->m_fMoveSpdPerSec;

	auto& targetInfo = m_pOwner->GetBrainCtrl()->GetTargetInfo();
	m_bIsValidTarget = targetInfo.IsValidTarget();

	if(m_bIsValidTarget)
	{
		// 타겟이 유효하다면 Nav에 위치 세팅
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(m_pOwner->GetBrainCtrl(), m_vDstWorldPos);
	}
	else
	{
		// 타겟이 무효하다면 현재 보고 있는 방향으로
		m_vDstWorldPos = m_pOwner->GetBrainCtrl()->GetLookInfoV();
	}

	return false;
}

bool UActionEventRuntime_MoveNavE::OnTick(float ElapsedTime, float DeltaTime)
{
	if (m_bIsValidTarget)
		return false;

	// 이동
	m_pOwner->AddMovementInput(m_vDstWorldPos, 1.f, true);
	return false;
}

void UActionEventRuntime_MoveNavE::OnEnd(bool bIsForLoopReset)
{
	// 루프 리셋 중이거나, 이동하지 않았다면
	if (bIsForLoopReset || !m_bIsValidTarget)
		return;

	// Nav 이동 종료
	m_pOwner->GetBrainCtrl()->StopMovement();
}
