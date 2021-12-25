#include "BrainController.h"
#include "SurveyInfo.h"
#include "../GameCharacter.h"
// #include "Client/GameContents/GameDatas/GameDataStructs.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DataLoadLib/DataFile/DataStructure/CCharacterData.h"

void ABrainController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	auto pGameCharacter = Cast<AGameCharacter>(InPawn);
	if (pGameCharacter)
		Init(pGameCharacter);
}

void ABrainController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ABrainController::SetSurveyInfo()
{
	m_SurveyInfo->Clear();

	const auto& pOwnerData = m_pOwner->GetData();
	const auto& pTeamInfo = m_pOwner->GetTeamInfo();
	if (!(pOwnerData && pTeamInfo))
		return;

	// 찾을 오브젝트 Flag
	int32 eccFlag = 0;
	eccFlag |= pTeamInfo->GetEccFlagFriendly();
	eccFlag |= pTeamInfo->GetEccFlagEnemy();

	// 나를 제외하고 찾는다
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(m_pOwner);

	// 주변 수색
	TArray<FHitResult> hitResults;
	FVector vPos = m_pOwner->GetActorLocation();
	const auto bHit = m_pOwner->GetWorld()->SweepMultiByObjectType(
		hitResults,
		vPos,
		vPos,
		m_pOwner->GetActorRotation().Quaternion(),
		eccFlag,
		FCollisionShape::MakeSphere(pOwnerData->Sight),
		queryParams
		);

#if ENABLE_DRAW_DEBUG
	// Draw Debug
	DrawDebugSphere(
		m_pOwner->GetWorld(),
		vPos,
		pOwnerData->Sight,
		20,
		bHit ? FColor::Green : FColor::Blue,
		false,
		0.1f);
#endif

	// 수색된 정보를 정리 및 세팅
	m_SurveyInfo->Clear();
	for (auto& hitResult : hitResults)
		m_SurveyInfo->AddInfo(m_pOwner, hitResult);
}

USurveyInfo* ABrainController::GetSurveyInfo()
{
	return m_SurveyInfo;
}

void ABrainController::StartAI()
{
	m_CurrAIBehaviorType = static_cast<EAIBehaviorTypes>(m_DefaultAIBehaviorType);
	m_bIsAIInCombo = false;
	
	BrainComponent->StartLogic();
}

void ABrainController::StopAI()
{
	BrainComponent->StopLogic(TEXT(""));
	
	m_CurrAIBehaviorType = static_cast<EAIBehaviorTypes>(m_DefaultAIBehaviorType);
	m_bIsAIInCombo = false;
}

void ABrainController::ResetAI()
{
	m_CurrAIBehaviorType = static_cast<EAIBehaviorTypes>(m_DefaultAIBehaviorType);
	m_bIsAIInCombo = false;

	BrainComponent->RestartLogic();
}

void ABrainController::SetDefaultAIBehavior(const EAIDefaultBehaviorType& behaviorType)
{
	m_DefaultAIBehaviorType = behaviorType;
}

const EAIDefaultBehaviorType& ABrainController::GetDefaultAIBehavior() const
{
	return m_DefaultAIBehaviorType;
}

void ABrainController::SetCurrAIBehavior(const EAIBehaviorTypes& behaviorType)
{
	m_CurrAIBehaviorType = behaviorType;
}

const EAIBehaviorTypes& ABrainController::GetCurrAIBehavior() const
{
	return m_CurrAIBehaviorType;
}

void ABrainController::SetAIInCombo(const bool& bCombo)
{
	m_bIsAIInCombo = bCombo;
}

void ABrainController::SetSpawnInfo(const FVector& vStartPoint, const FQuat& qStartLook, const USplineComponent* const& pSplineComp)
{
	if (!m_pSpawnInfo)
		m_pSpawnInfo = new GameCharacterSpawnInfo();
	
	m_pSpawnInfo->Init(GetOwner(), vStartPoint, qStartLook, pSplineComp);
  }

GameCharacterSpawnInfo* const& ABrainController::GetSpawnInfo() const
{
	return m_pSpawnInfo;
}

void ABrainController::ClearSpawnInfo()
{
	if (m_pSpawnInfo)
	{
		delete m_pSpawnInfo;
		m_pSpawnInfo = nullptr;
	}
}

const bool& ABrainController::IsAIInCombo() const
{
	return m_bIsAIInCombo;
}
