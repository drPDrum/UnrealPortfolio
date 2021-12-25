#include "BrainController.h"
#include "../../GameCharacter/GameCharacter.h"
#include "SurveyInfo.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionController.h"
#include "Framework/Managers/Managers.h"

ABrainController::ABrainController()
{
	// For All
	m_pOwner = nullptr;
	m_pOwnerData = nullptr;

	m_qLookWorldDir = FQuat::Identity;	
	m_vInputWorldDir = FVector::ZeroVector;

	// For AI
	m_DefaultAIBehaviorType = EAIDefaultBehaviorType::None;
	m_CurrAIBehaviorType = EAIBehaviorTypes::None;
	m_pSpawnInfo = nullptr;
	m_bIsAIInCombo = false;
	m_AIBT = nullptr;

	// For AI :: Init m_AIBB(Blackboard)
	static ConstructorHelpers::FObjectFinder<UBlackboardData> objFinder_BB(		
		TEXT("BlackboardData'/Game/GameContents/Characters/Char_Common/AI/AIBB_GameCharacter.AIBB_GameCharacter'"));

	if (objFinder_BB.Succeeded())
		m_AIBB = objFinder_BB.Object;

	// For Input
	m_IsOrderByUserInput = false;
	m_AxisTriggerType = ETriggerAxisTypes::NoUse;
	m_fInputScale = 0.f;
	m_bRefreshInput = true;
}

ABrainController::~ABrainController()
{
	ClearSpawnInfo();
}

void ABrainController::Init(AGameCharacter* pOwner)
{
	m_pOwner = pOwner;
	m_qLookWorldDir = m_pOwner->GetActorRotation().Quaternion();

	auto pOwnerData = pOwner->GetData();

	if (!(pOwner && pOwnerData))
		return;
	
	if (!m_SurveyInfo)
		m_SurveyInfo = NewObject<USurveyInfo>(this);


	m_pOwnerData = m_pOwner->GetData();

	if (!m_AIBT || m_pOwnerData != pOwnerData)
	{
		m_pOwnerData = pOwnerData;
		
		// Load BehaviorTree
		auto pathFile_AIBT = FPaths::Combine(m_pOwnerData->RefPath, TEXT("AI"), m_pOwnerData->FileNameAIBT);
		auto pathRef_AIBT = FString::Printf(TEXT("BehaviorTree'%s.%s'"), *pathFile_AIBT, *(m_pOwnerData->FileNameAIBT));
		auto pAIBT = Managers::Asset->LoadObj<UBehaviorTree>(*pathRef_AIBT);

		// Init m_AIBT(BehaviorTree)
		m_AIBT = Cast<UBehaviorTree>(pAIBT);
	}

	if (m_AIBB && UseBlackboard(m_AIBB, Blackboard) && RunBehaviorTree(m_AIBT))
	{
		// 일단 멈춘다. AGameModeBase::OnStart에서 켜주자
		StopAI();
	}
	else
	{
		LOG_ERROR(TEXT("AI Run Failed :: %s"), *m_pOwnerData->Name);
	}
}

AGameCharacter* ABrainController::GetOwner()
{
	return m_pOwner;
}

void ABrainController::OnOwnerTick(float deltaTime)
{
	RefreshInputDir(false);
}

void ABrainController::RefreshInputDir(bool bForced)
{
	if (!(bForced || m_bRefreshInput))
		return;

	m_bRefreshInput = false;
	
	// Update Input Dir
	auto vInput = FVector(m_vInputScaleMove.Y, m_vInputScaleMove.X ,0.f);
	auto fInputSize = vInput.Size();
	if (fInputSize > 0.f)
	{
		vInput = vInput * (1.f / fInputSize);
		m_fInputScale = FMath::Clamp(fInputSize, 0.f,1.f);
		
		m_vInputWorldDir = m_qLookWorldDir * vInput;
		m_qInputWorldDir = FRotationMatrix::MakeFromZX(m_qLookWorldDir.GetUpVector(),m_vInputWorldDir).ToQuat();

		float angle = FMath::RadiansToDegrees((acosf(FVector::DotProduct(m_qLookWorldDir.GetForwardVector(), m_vInputWorldDir))));
		if (angle < 45.f)
			m_AxisTriggerType = ETriggerAxisTypes::Front;
		else if (angle < 135.f)
			m_AxisTriggerType = ETriggerAxisTypes::Side;
		else
			m_AxisTriggerType = ETriggerAxisTypes::Back;
	}
	else
	{
		m_vInputWorldDir = m_qLookWorldDir.GetForwardVector();
		m_qInputWorldDir = m_qLookWorldDir;
		m_AxisTriggerType = ETriggerAxisTypes::None;
		m_fInputScale = 0.f;
	}	
}

const FQuat& ABrainController::GetAxisWorldDirQ()
{
	return m_qInputWorldDir;
}

const FVector& ABrainController::GetAxisWorldDirV()
{
	return m_vInputWorldDir;
}

const ETriggerAxisTypes& ABrainController::GetAxisTriggerType()
{
	return m_AxisTriggerType;
}

const float& ABrainController::GetAxisPower()
{
	return m_fInputScale;
}

void ABrainController::SetLookInfo(FQuat qLookWorldDir)
{
	m_bRefreshInput =  m_bRefreshInput || m_qLookWorldDir != qLookWorldDir;
	m_qLookWorldDir = qLookWorldDir;
}

const FQuat& ABrainController::GetLookInfoQ() const
{
	return m_qLookWorldDir;
}

const FVector& ABrainController::GetLookInfoV() const
{
	return m_vInputWorldDir;
}

bool ABrainController::SetTarget(const float& radius, const EGameCharacterFindTargetTypes& targetType, const EGameCharacterStateTypes& stateType)
{
	m_TargetInfo.Clear();

	// 찾을 오브젝트 Flag
	int32 eccFlag = 0;
	switch(targetType)
	{
		case EGameCharacterFindTargetTypes::Friendly:
			eccFlag = m_pOwner->GetTeamInfo()->GetEccFlagFriendly();
			break;
		case EGameCharacterFindTargetTypes::Enemy:
			eccFlag = m_pOwner->GetTeamInfo()->GetEccFlagEnemy();
			break;
		case EGameCharacterFindTargetTypes::Both:
			eccFlag = m_pOwner->GetTeamInfo()->GetEccFlagFriendly();
			eccFlag |= m_pOwner->GetTeamInfo()->GetEccFlagEnemy();
			break;
	}
	
	// 나를 제외하고 찾는다
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(m_pOwner);

	TArray<FHitResult> results; 
	
	FVector vPos = m_pOwner->GetActorLocation();
	const auto bHit = m_pOwner->GetWorld()->SweepMultiByObjectType(
		results,
		vPos,
		vPos,
		m_pOwner->GetActorRotation().Quaternion(),
		eccFlag,
		FCollisionShape::MakeSphere(radius),
		queryParams
		);

	if (bHit)
	{
#if ENABLE_DRAW_DEBUG
		DrawDebugSphere(
			m_pOwner->GetWorld(),
			m_pOwner->GetActorLocation(),
			radius,
			20,
			FColor::Green,
			false,
			1.f);
#endif

		FHitResult* pHitResult = nullptr;
		
		if (m_fInputScale > 0.1f)
		{
			// Input 각도 우선
			auto fCos_Max = -1.f;
			for (int i = 0 ; i < results.Num(); ++i)
			{
				auto& result = results[i];
				auto vToTarget = (result.ImpactPoint - m_pOwner->GetActorLocation());
				vToTarget.Normalize();
				
				auto fCos = FVector::DotProduct(m_vInputWorldDir, vToTarget);
				if (fCos_Max <= fCos)
				{
					fCos_Max = fCos;
					pHitResult = &result;
				}
			}
		}
		else
		{
			// 거리 우선
			auto fSqDist_Min = TNumericLimits<float>::Max();
			for (int i = 0 ; i < results.Num(); ++i)
			{
				auto& result = results[i];
				auto fSqDist = FVector::DistSquared(m_pOwner->GetActorLocation(), result.ImpactPoint);
				if (fSqDist_Min >= fSqDist)
				{
					fSqDist_Min = fSqDist;
					pHitResult = &result;
				}
			}
		}

		if (pHitResult)
			SetTarget(*pHitResult);
	}
#if ENABLE_DRAW_DEBUG
	else
	{
		DrawDebugSphere(
			m_pOwner->GetWorld(),
			m_pOwner->GetActorLocation(),
			radius,
			20,
			FColor::Red,
			false,
			1.f);
	}
#endif
	
	return bHit;
}

void ABrainController::SetTarget(const FHitResult& hitResult)
{
	SetTarget(hitResult.GetActor(), HitResultToBodyInst(hitResult));
}

void ABrainController::SetTarget(AActor* pTarget, FBodyInstance* pBody)
{
	m_TargetInfo.m_pTarget = pTarget;
	m_TargetInfo.m_pBody = pBody;

	if (pBody)
		m_TargetInfo.m_Type = ETargetFindingResultTypes::Body;
	else if (pTarget)
		m_TargetInfo.m_Type = ETargetFindingResultTypes::Actor;
	else
		m_TargetInfo.m_Type = ETargetFindingResultTypes::None;
}

void ABrainController::SetTarget(const FVector& vTargetLocation)
{
	m_TargetInfo.m_Type = ETargetFindingResultTypes::Pos;
	m_TargetInfo.m_vLocation = vTargetLocation;
}

const GameCharacterTargetInfo& ABrainController::GetTargetInfo() const
{
	return m_TargetInfo;
}
