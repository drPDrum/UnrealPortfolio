#include "ActionEventRuntime_HitShape.h"
#include "CollisionDebugDrawingPublic.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "Framework/Managers/Managers.h"
#include "Framework/Managers/FXManager/FXActor.h"
#include "Framework/Managers/FXManager/FXManager.h"

UActionEventRuntime_HitShape::UActionEventRuntime_HitShape()
{
	m_pSoundBase = nullptr;
}

UActionEventRuntime_HitShape::~UActionEventRuntime_HitShape()
{
	if (m_pFXContainer && Managers::IsInit())
		Managers::FX->ReleaseFX(m_pFXContainer);

	m_pFXContainer = nullptr;
	m_pSoundBase = nullptr;
	m_Shapes.Empty();
}

void UActionEventRuntime_HitShape::Init(FActionEventData_HitShape& Data, AGameCharacter* Owner)
{
	ActionEventRuntimeBase<FActionEventData_HitShape>::Init(Data, Owner);

	m_Shapes.Empty();

	// Init FX
	auto BPClass = Managers::Asset->LoadBP<AFXActor>(*(m_pData->m_ResPath_FX));
	if (BPClass)
		m_pFXContainer = Managers::FX->RegistFX(BPClass, 5);
	else
		m_pFXContainer = nullptr;

	// Init SFX
	m_pSoundBase = Managers::Asset->LoadObj<USoundBase>(*m_pData->m_ResPath_SFX);

	// Init Shape
	auto& hitInfos = m_pData->m_hitInfos;
	for (int i = 0 ; i < hitInfos.Num(); ++i)
	{
		auto& hitInfo = hitInfos[i];
		switch(hitInfo.m_CastType)
		{
			case ECastTypes::Target_Box:
			case ECastTypes::MyPos_Box:
				m_Shapes.Add(FCollisionShape::MakeBox(hitInfo.m_Size * 0.5f));
				break;
			case ECastTypes::Target_Sphere:
			case ECastTypes::MyPos_Sphere:
				m_Shapes.Add(FCollisionShape::MakeSphere(hitInfo.m_Size.X));
				break;
		}
	}
}

bool UActionEventRuntime_HitShape::OnStart(float ElapsedTime)
{
	// Base Dir
	FQuat qFoward = m_pOwner->GetActorRotation().Quaternion();
	
	// ECCFlag
	int32 ECCFlag = 0;
	switch (m_pData->m_TargetType)
	{
		case EGameCharacterFindTargetTypes::Friendly:
			ECCFlag = m_pOwner->GetTeamInfo()->GetEccFlagFriendly();
			break;
		case EGameCharacterFindTargetTypes::Enemy:
			ECCFlag = m_pOwner->GetTeamInfo()->GetEccFlagEnemy(); 
			break;
		case EGameCharacterFindTargetTypes::Both:
			ECCFlag = m_pOwner->GetTeamInfo()->GetEccFlagFriendly(); 
			ECCFlag |= m_pOwner->GetTeamInfo()->GetEccFlagEnemy(); 
			break;
	}

	// Cast
	TSet<AActor*> set_AleadyExcute;
	TArray<FHitResult> results;

#if ENABLE_DRAW_DEBUG
	TArray<FHitResult> results_Debug;
#endif

	auto& hitInfos = m_pData->m_hitInfos;
	for (int i = 0 ; i < hitInfos.Num(); ++i)
	{
		results.Empty();

		// Start Pos
		auto& hitInfo = hitInfos[i];

		FVector vStart;
		FVector vOffsetPos = qFoward * hitInfo.m_vOffsetPos;
			
		switch(hitInfo.m_CastType)
		{
		case ECastTypes::MyPos_Sphere:
		case ECastTypes::MyPos_Box:
			vStart = m_pOwner->GetActorLocation();
			break;
		case ECastTypes::Target_Sphere:
		case ECastTypes::Target_Box:
			if (m_pOwner->GetBrainCtrl()->GetTargetInfo().IsValidTarget())
				vStart = m_pOwner->GetBrainCtrl()->GetTargetInfo().GetLocation();
			else
				vStart = m_pOwner->GetActorLocation();
			break;
		}

		vStart += vOffsetPos;

		// End Pos
		FVector vEnd = vStart + (qFoward * hitInfo.m_qOffsetRot).GetForwardVector() * hitInfo.m_fDist;

		// Shape
		auto& shape = m_Shapes[i];

		// Debug Draw
#if ENABLE_DRAW_DEBUG
		switch (shape.ShapeType)
		{
		case ECollisionShape::Type::Box:
			DrawBoxSweeps(m_pOwner->GetWorld(), vStart, vEnd,shape.GetBox(), qFoward, results_Debug, 1.f);
			break;
		case ECollisionShape::Type::Sphere:
			DrawSphereSweeps(m_pOwner->GetWorld(), vStart, vEnd, shape.GetSphereRadius(), results_Debug, 1.f);
			break;
		default:
			break;
		}
#endif
		
		// Sweep
		if (m_pOwner->GetWorld()->SweepMultiByObjectType(results, vStart, vEnd, qFoward, ECCFlag, shape))
		{
			for (int j = 0 ; j < results.Num() ; ++j)
			{
				auto& result = results[j];
				auto pActor = result.GetActor();

				if (set_AleadyExcute.Contains(pActor))
					continue;

				set_AleadyExcute.Add(pActor);

				auto pTarget = Cast<AGameCharacter>(pActor);
				if (pTarget)
				{
					pTarget->Hit(
						m_pOwner,
						result.ImpactPoint,
						FRotationMatrix::MakeFromZX(m_pOwner->GetActorUpVector(), result.ImpactNormal).ToQuat(),
						m_pSoundBase,
						m_pFXContainer);
				}
			}
		}
	}

	return false;
}