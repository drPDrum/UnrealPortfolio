// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventRuntime_MoveInputU.h"
#include "../../BrainControl/BrainController.h"

UActionEventRuntime_MoveInputU::UActionEventRuntime_MoveInputU()
{
}

UActionEventRuntime_MoveInputU::~UActionEventRuntime_MoveInputU()
{
}

bool UActionEventRuntime_MoveInputU::OnStart(float ElapsedTime)
{
	// 속도 세팅
	m_pOwner->GetCharacterMovement()->MaxWalkSpeed = m_pData->m_fMoveSpdPerSec;

	// 방향 업데이트
	m_vDstWorldDir = m_pOwner->GetBrainCtrl()->GetAxisWorldDirV();

	return false;
}

bool UActionEventRuntime_MoveInputU::OnTick(float elapsedTime, float deltaTime)
{
	// 방향 업데이트
	if (m_pData->m_bUseUpdateAngleSpd)
	{
		auto vDirToTarget =  m_pOwner->GetBrainCtrl()->GetAxisWorldDirV();

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
				FVector termTwo = vDirToTarget		 * (FGenericPlatformMath::Sin(fRadian * (      t)) / sinOmega);

				m_vDstWorldDir = termOne + termTwo;
			}
		}
	}
	else
	{
		m_vDstWorldDir = m_pOwner->GetBrainCtrl()->GetAxisWorldDirV();
	}

	// 이동
	if (m_pData->m_bUseInputScale)
		m_pOwner->AddMovementInput(m_vDstWorldDir, m_pOwner->GetBrainCtrl()->GetAxisPower(), true);
	else
		m_pOwner->AddMovementInput(m_vDstWorldDir, 1.f, true);
	
	return false;
}