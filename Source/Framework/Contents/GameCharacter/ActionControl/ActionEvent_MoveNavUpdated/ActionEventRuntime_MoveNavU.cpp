#include "ActionEventRuntime_MoveNavU.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UActionEventRuntime_MoveNavU::UActionEventRuntime_MoveNavU()
{
}

UActionEventRuntime_MoveNavU::~UActionEventRuntime_MoveNavU()
{
}

bool UActionEventRuntime_MoveNavU::OnStart(float ElapsedTime)
{
	// 속도 세팅
	m_pOwner->GetCharacterMovement()->MaxWalkSpeed = m_pData->m_fMoveSpdPerSec;

	m_TargetInfo = m_pOwner->GetBrainCtrl()->GetTargetInfo();
	if(m_TargetInfo.IsValidTarget())
	{
		// 타겟이 유효하다면 Nav에 위치 세팅
		m_bIsActiveNav = true;
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(m_pOwner->GetBrainCtrl(), m_TargetInfo.GetLocation());
	}
	else
	{
		m_bIsActiveNav = false;
	}

	return false;
}

bool UActionEventRuntime_MoveNavU::OnTick(float ElapsedTime, float DeltaTime)
{
	m_TargetInfo = m_pOwner->GetBrainCtrl()->GetTargetInfo();
	if(m_TargetInfo.IsValidTarget())
	{
		// 타겟이 유효하다면 Nav에 위치 세팅
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(m_pOwner->GetBrainCtrl(), m_TargetInfo.GetLocation());
	}
	else
	{
		if (m_bIsActiveNav)
		{
			m_bIsActiveNav = false;
			m_pOwner->GetBrainCtrl()->StopMovement();
		}
		
		// 타겟이 무효하다면 현재 보고 있는 방향으로
		m_pOwner->AddMovementInput(m_pOwner->GetBrainCtrl()->GetLookInfoV(), 1.f, true);
	}

	// 이동
	return false;
}

void UActionEventRuntime_MoveNavU::OnEnd(bool bIsForLoopReset)
{
	// 루프 리셋 중이거나, 이동하지 않았다면 호출 스택 탈출
	if (bIsForLoopReset || !m_bIsActiveNav)
		return;

	// Nav 이동 종료
	m_pOwner->GetBrainCtrl()->StopMovement();
}
