// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventRuntime_MoveTargetE.h"
#include "../../ActionControl/ActionController.h"

UActionEventRuntime_MoveTargetE::UActionEventRuntime_MoveTargetE()
{
}

UActionEventRuntime_MoveTargetE::~UActionEventRuntime_MoveTargetE()
{
	m_pActionRuntime_OnReachTarget = nullptr;
}

void UActionEventRuntime_MoveTargetE::InitLinkedActionRuntime(const TMap<FString, UActionRuntime*>& map_ActionRuntimes)
{
	ActionEventDurationRuntimeBase<FActionEventData_MoveTargetE>::InitLinkedActionRuntime(map_ActionRuntimes);

	if (!m_pData->OnReachTargetPlay.IsEmpty() && map_ActionRuntimes.Contains(m_pData->OnReachTargetPlay))
		m_pActionRuntime_OnReachTarget = map_ActionRuntimes[m_pData->OnReachTargetPlay];
}

bool UActionEventRuntime_MoveTargetE::OnStart(float ElapsedTime)
{
	// 속도 세팅
	m_pOwner->GetCharacterMovement()->MaxWalkSpeed = m_pData->m_fMoveSpdPerSec;

	// 방향 업데이트
	m_TargetInfo = m_pOwner->GetBrainCtrl()->GetTargetInfo();
	if (m_TargetInfo.IsValidTarget())
	{
		m_vDstWorldPos = m_TargetInfo.GetLocation();
		m_vDstWorldDir = m_vDstWorldPos - m_pOwner->GetActorLocation();
		m_vDstWorldDir = FVector::VectorPlaneProject(m_vDstWorldDir, m_pOwner->GetActorUpVector());

		m_vDstWorldDir.Normalize();

		// 도달 체크 및 도달 액션 실행
		if (m_pActionRuntime_OnReachTarget && (CheckSweep() || CheckDistance()))
		{
			m_pOwner->GetActionCtrl()->PlayAction(m_pActionRuntime_OnReachTarget);
			return true;
		}
	}
	else
	{
		m_vDstWorldDir = m_pOwner->GetBrainCtrl()->GetAxisWorldDirV();
	}

	return false;
}

bool UActionEventRuntime_MoveTargetE::OnTick(float ElapsedTime, float DeltaTime)
{
	// 이동
	m_pOwner->AddMovementInput(m_vDstWorldDir, 1.f, true);
	
	// 도달 체크 및 도달 액션 실행
	if (m_pActionRuntime_OnReachTarget && m_TargetInfo.IsValidTarget() && (CheckSweep() || CheckDistance()))
	{
		m_pOwner->GetActionCtrl()->PlayAction(m_pActionRuntime_OnReachTarget);
		return true;
	}
	
	return false;
}

bool UActionEventRuntime_MoveTargetE::CheckSweep() const
{
	FHitResult hit;

	auto vStart = m_pOwner->GetActorLocation();
	auto vEnd = vStart + m_vDstWorldDir * m_pData->m_fOnReachDist;

	return m_TargetInfo.Sweep(hit, vStart, vEnd, m_pOwner->GetActorRotation().Quaternion(), m_pOwner->GetCapsuleComponent()->GetCollisionShape());
}

bool UActionEventRuntime_MoveTargetE::CheckDistance() const
{
	auto vToTarget = m_vDstWorldPos - m_pOwner->GetActorLocation();
	vToTarget = FVector::VectorPlaneProject(vToTarget, m_pOwner->GetActorUpVector());

	auto fReachDist = m_pOwner->GetCapsuleComponent()->GetScaledCapsuleRadius()
							+ (m_pData->m_fOnReachDist * m_pOwner->GetActorScale3D().X);
	
	return vToTarget.SizeSquared() < FMath::Square(fReachDist);
}