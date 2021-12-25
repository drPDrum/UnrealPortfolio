// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventRuntime_RotateVel.h"

UActionEventRuntime_RotateVel::UActionEventRuntime_RotateVel()
{
}

UActionEventRuntime_RotateVel::~UActionEventRuntime_RotateVel()
{
}

bool UActionEventRuntime_RotateVel::OnTick(float ElapsedTime, float DeltaTime)
{
	// 방향 구하기
	auto vMoveVel = FVector::VectorPlaneProject(m_pOwner->GetMovementComponent()->Velocity, FVector::UpVector);
	auto fMoveSize = vMoveVel.SizeSquared();

	if (fMoveSize > 0)
	{
		vMoveVel *= (1.f / FMath::Sqrt(fMoveSize));

		// Todo :: 개별 중력 이라면 코드 변경 필요
		FQuat qDstWorldDir = FRotationMatrix::MakeFromZX(FVector::UpVector, vMoveVel).ToQuat();

		// 회전
		auto fRadian = qDstWorldDir.AngularDistance(m_pOwner->GetActorQuat());
		if (fRadian > 0.f)
		{
			auto fFactor = FMath::Clamp(m_pData->m_fRotateSpdPerSec * DeltaTime / FMath::RadiansToDegrees(fRadian), 0.f, 1.f);
			m_pOwner->SetActorRotation(FQuat::Slerp(m_pOwner->GetActorQuat(), qDstWorldDir, fFactor));
		}
	}
	
	return false;
}

