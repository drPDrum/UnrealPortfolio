#include "ActionEventRuntime_RotateTargetU.h"
#include "../../BrainControl/BrainController.h"

UActionEventRuntime_RotateTargetU::UActionEventRuntime_RotateTargetU()
{
	
}

UActionEventRuntime_RotateTargetU::~UActionEventRuntime_RotateTargetU()
{
	
}

bool UActionEventRuntime_RotateTargetU::OnTick(float elapsedTime, float deltaTime)
{
	// 방향 구하기
	FQuat qDstWorldDir;
	const auto& targetInfo = m_pOwner->GetBrainCtrl()->GetTargetInfo();
	if (targetInfo.IsValidTarget())
	{
		// Todo :: 개별 중력 이라면 코드 변경 필요

		auto vForward = targetInfo.GetLocation() - m_pOwner->GetActorLocation();
		vForward = FVector::VectorPlaneProject(vForward, FVector::UpVector);
		vForward.Normalize();

		qDstWorldDir = FRotationMatrix::MakeFromZX(FVector::UpVector, vForward).ToQuat();
	}
	else
	{
		qDstWorldDir = m_pOwner->GetBrainCtrl()->GetAxisWorldDirQ();
	}

	// 회전
	auto fRadian = qDstWorldDir.AngularDistance(m_pOwner->GetActorQuat());
	if (fRadian > 0.f)
	{
		auto fFactor = FMath::Clamp(m_pData->m_fRotateSpdPerSec * deltaTime / FMath::RadiansToDegrees(fRadian), 0.f, 1.f);
		m_pOwner->SetActorRotation(FQuat::Slerp(m_pOwner->GetActorQuat(), qDstWorldDir, fFactor));
	}
	
	return false;
}