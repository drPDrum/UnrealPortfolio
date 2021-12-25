#include "ActionEventRuntime_RotateTargetE.h"
#include "../../BrainControl/BrainController.h"

UActionEventRuntime_RotateTargetE::UActionEventRuntime_RotateTargetE()
{
	
}

UActionEventRuntime_RotateTargetE::~UActionEventRuntime_RotateTargetE()
{
	
}

bool UActionEventRuntime_RotateTargetE::OnStart(float elapsedTime)
{
	// 방향 저장
	const auto& targetInfo = m_pOwner->GetBrainCtrl()->GetTargetInfo();
	if (targetInfo.IsValidTarget())
	{
		// Todo :: 개별 중력 이라면 코드 변경 필요 // FVector::UpVector -> 각 AGameCharacter의 Up
		auto vForward = targetInfo.GetLocation() - m_pOwner->GetActorLocation();
		vForward = FVector::VectorPlaneProject(vForward, FVector::UpVector);
		vForward.Normalize();

		m_qDstWorldDir = FRotationMatrix::MakeFromZX(FVector::UpVector, vForward).ToQuat();
	}
	else
	{
		m_qDstWorldDir = m_pOwner->GetBrainCtrl()->GetAxisWorldDirQ();
	}

	return false;
}

bool UActionEventRuntime_RotateTargetE::OnTick(float elapsedTime, float deltaTime)
{
	// 회전
	auto fRadian = m_qDstWorldDir.AngularDistance(m_pOwner->GetActorQuat());
	if (fRadian > 0.f)
	{
		auto fFactor = FMath::Clamp(m_pData->m_fRotateSpdPerSec * deltaTime / FMath::RadiansToDegrees(fRadian), 0.f, 1.f);
		m_pOwner->SetActorRotation(FQuat::Slerp(m_pOwner->GetActorQuat(), m_qDstWorldDir, fFactor));
	}
	
	return false;
}