#include "ActionEventRuntime_RotateInputE.h"
#include "../../BrainControl/BrainController.h"

UActionEventRuntime_RotateInputE::UActionEventRuntime_RotateInputE()
{
	
}

UActionEventRuntime_RotateInputE::~UActionEventRuntime_RotateInputE()
{
	
}

bool UActionEventRuntime_RotateInputE::OnStart(float elapsedTime)
{
	// 방향 저장
	m_qDstWorldDir = m_pOwner->GetBrainCtrl()->GetAxisWorldDirQ();

	return false;
}

bool UActionEventRuntime_RotateInputE::OnTick(float elapsedTime, float deltaTime)
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