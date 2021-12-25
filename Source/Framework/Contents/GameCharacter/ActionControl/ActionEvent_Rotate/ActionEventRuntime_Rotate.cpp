#include "ActionEventRuntime_Rotate.h"
#include "../../BrainControl/BrainController.h"

UActionEventRuntime_Rotate::UActionEventRuntime_Rotate()
{
}

UActionEventRuntime_Rotate::~UActionEventRuntime_Rotate()
{
}

void UActionEventRuntime_Rotate::Init(FActionEventData_Rotate& Data, AGameCharacter* Owner)
{
	ActionEventDurationRuntimeBase<FActionEventData_Rotate>::Init(Data, Owner);

	switch(m_pData->m_RotateType)
	{
	case ERotateTypes::Left:	m_fRotateRadianSpdPerSec = -FMath::DegreesToRadians(m_pData->m_fRotateSpdPerSec); break;
	case ERotateTypes::Right:	m_fRotateRadianSpdPerSec = FMath::DegreesToRadians(m_pData->m_fRotateSpdPerSec); break;
	}
}

bool UActionEventRuntime_Rotate::OnTick(float elapsedTime, float deltaTime)
{
	auto fDeltaDegree = m_pData->m_fRotateSpdPerSec * deltaTime;
	
	// Todo :: 개별 중력 이라면 코드 변경 필요
	FQuat newQuat = FQuat(FVector::UpVector, fDeltaDegree);
	m_pOwner->SetActorRotation(m_pOwner->GetActorQuat() * newQuat);
	
	return false;
}