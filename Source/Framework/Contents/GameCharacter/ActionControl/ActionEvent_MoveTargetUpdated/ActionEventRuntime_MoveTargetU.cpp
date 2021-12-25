// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventRuntime_MoveTargetU.h"
#include "../../ActionControl/ActionController.h"

UActionEventRuntime_MoveTargetU::UActionEventRuntime_MoveTargetU()
{
}

UActionEventRuntime_MoveTargetU::~UActionEventRuntime_MoveTargetU()
{
	m_pActionRuntime_OnReachTarget = nullptr;
}

void UActionEventRuntime_MoveTargetU::InitLinkedActionRuntime(const TMap<FString, UActionRuntime*>& map_ActionRuntimes)
{
	ActionEventDurationRuntimeBase<FActionEventData_MoveTargetU>::InitLinkedActionRuntime(map_ActionRuntimes);

	if (!m_pData->OnReachTargetPlay.IsEmpty() && map_ActionRuntimes.Contains(m_pData->OnReachTargetPlay))
		m_pActionRuntime_OnReachTarget = map_ActionRuntimes[m_pData->OnReachTargetPlay];
}

bool UActionEventRuntime_MoveTargetU::OnStart(float ElapsedTime)
{
	// 속도 세팅
	m_pOwner->GetCharacterMovement()->MaxWalkSpeed = m_pData->m_fMoveSpdPerSec;

	// 방향 업데이트
	m_TargetInfo = m_pOwner->GetBrainCtrl()->GetTargetInfo();
	if (m_TargetInfo.IsValidTarget())
	{
		m_vDstWorldDir = m_TargetInfo.GetLocation() - m_pOwner->GetActorLocation();
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

bool UActionEventRuntime_MoveTargetU::OnTick(float elapsedTime, float deltaTime)
{
	// 방향 업데이트
	FVector vDirToTarget;
	
	m_TargetInfo = m_pOwner->GetBrainCtrl()->GetTargetInfo();
	if (m_TargetInfo.IsValidTarget())
	{
		vDirToTarget = m_TargetInfo.GetLocation() - m_pOwner->GetActorLocation();
		vDirToTarget = FVector::VectorPlaneProject(vDirToTarget, m_pOwner->GetActorUpVector());

		vDirToTarget.Normalize();
	}
	else
	{
		vDirToTarget = m_pOwner->GetBrainCtrl()->GetAxisWorldDirV();
	}

	if (m_pData->m_bUseUpdateAngleSpd)
	{
		// 목표와 기존 이동 방향의 각도
		auto fRadian = FMath::Acos(FVector::DotProduct(m_vDstWorldDir,vDirToTarget));
		if (fRadian != 0.f)
		{
			// 이번에 회전할 각도
			auto fDeltaRadian = FMath::DegreesToRadians(m_pData->m_fUpdateAngleSpd * deltaTime);

			// 회전할 각도가 목표보다 크면 대입
			if (fDeltaRadian > fRadian)
			{
				m_vDstWorldDir = vDirToTarget;
			}
			// 회전할 각도가 목표보다 작으면 구형 보간 후 대입
			else
			{
				auto t =  fDeltaRadian / fRadian;

				float sinOmega = FGenericPlatformMath::Sin(fRadian);
				FVector termOne = m_vDstWorldDir * (FGenericPlatformMath::Sin(fRadian * (1.0 - t)) / sinOmega);
				FVector termTwo = vDirToTarget	 * (FGenericPlatformMath::Sin(fRadian * (      t)) / sinOmega);

				m_vDstWorldDir = termOne + termTwo;
			}
		}
	}
	else
	{
		m_vDstWorldDir = vDirToTarget;
	}
	
	// 이동
	m_pOwner->AddMovementInput(m_vDstWorldDir, 1.f, true);
	
	// 도달 체크 및 도달 액션 실행
	if (m_TargetInfo.IsValidTarget() && m_pActionRuntime_OnReachTarget && (CheckSweep() || CheckDistance()))
	{
		m_pOwner->GetActionCtrl()->PlayAction(m_pActionRuntime_OnReachTarget);
		return true;
	}
	
	return false;
}

bool UActionEventRuntime_MoveTargetU::CheckSweep() const
{
	FHitResult hit;

	auto vStart = m_pOwner->GetActorLocation();
	auto vEnd = vStart + m_vDstWorldDir * m_pData->m_fOnReachDist;

	return m_TargetInfo.Sweep(hit, vStart, vEnd, m_pOwner->GetActorRotation().Quaternion(), m_pOwner->GetCapsuleComponent()->GetCollisionShape());
}

bool UActionEventRuntime_MoveTargetU::CheckDistance() const
{
	auto vToTarget = m_TargetInfo.GetLocation() - m_pOwner->GetActorLocation();
	vToTarget = FVector::VectorPlaneProject(vToTarget, m_pOwner->GetActorUpVector());

	auto fReachDist = m_pOwner->GetCapsuleComponent()->GetScaledCapsuleRadius()
							+ (m_pData->m_fOnReachDist * m_pOwner->GetActorScale3D().X);
	
	return vToTarget.SizeSquared() < FMath::Square(fReachDist);
}