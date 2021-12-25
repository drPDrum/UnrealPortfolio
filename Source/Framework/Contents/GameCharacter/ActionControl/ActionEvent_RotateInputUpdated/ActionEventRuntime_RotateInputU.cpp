#include "ActionEventRuntime_RotateInputU.h"
#include "../../BrainControl/BrainController.h"

UActionEventRuntime_RotateInputU::UActionEventRuntime_RotateInputU()
{
	
}

UActionEventRuntime_RotateInputU::~UActionEventRuntime_RotateInputU()
{
	
}

bool UActionEventRuntime_RotateInputU::OnTick(float elapsedTime, float deltaTime)
{
	// 방향 구하기
	auto qDstWorldDir = m_pOwner->GetBrainCtrl()->GetAxisWorldDirQ();

	// 회전
	auto fRadian = qDstWorldDir.AngularDistance(m_pOwner->GetActorQuat());
	if (fRadian > 0.f)
	{
		auto fFactor = FMath::Clamp(m_pData->m_fRotateSpdPerSec * deltaTime / FMath::RadiansToDegrees(fRadian), 0.f, 1.f);
		m_pOwner->SetActorRotation(FQuat::Slerp(m_pOwner->GetActorQuat(), qDstWorldDir, fFactor));
	}
	
	return false;
}