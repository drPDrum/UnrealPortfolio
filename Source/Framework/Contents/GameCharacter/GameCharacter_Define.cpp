#include "GameCharacter_Define.h"

#include "GameCharacter.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"

GameCharacterTeamInfo::GameCharacterTeamInfo(const int& idx_Mine, TArray<int> idxes_Friendly, TArray<int> idxes_Enemy)
{
	Idx_Mine = idx_Mine;

	CollisionProfile_Mine = ConvertIdxToCollisionProfileName(Idx_Mine);
	ECC_Mine = ConvertIdxToEcc(Idx_Mine);
	ECCFlag_Mine = ECC_TO_BITFIELD(ECC_Mine);

	// 아군 팀 정보 정리
	Idxes_Team_Friendly = idxes_Friendly;

	IdxFlag_Friendly = 1 << Idx_Mine;
	ECCFlag_Friendly = ECCFlag_Mine;
	
	for (int i = 0 ; i < Idxes_Team_Friendly.Num(); ++i)
	{
		auto& idx = Idxes_Team_Friendly[i];
		IdxFlag_Friendly |= (1 << idx);
		ECCFlag_Friendly |= ECC_TO_BITFIELD(ConvertIdxToEcc(idx));
	}

	// 적 팀 정보 정리
	Idxes_Team_Enemy = idxes_Enemy;
	
	IdxFlag_Enemy = 0;
	ECCFlag_Enemy = 0;
	
	for (int i = 0 ; i < Idxes_Team_Enemy.Num(); ++i)
	{
		auto& idx = Idxes_Team_Enemy[i];
		IdxFlag_Enemy |= (1 << idx);
		ECCFlag_Enemy |= ECC_TO_BITFIELD(ConvertIdxToEcc(idx));
	}
}

GameCharacterTeamInfo::~GameCharacterTeamInfo()
{
}

const int& GameCharacterTeamInfo::GetIdx() const
{
	return Idx_Mine;
}

const FString& GameCharacterTeamInfo::GetCollisionProfile() const
{
	return CollisionProfile_Mine;
}

const ECollisionChannel& GameCharacterTeamInfo::GetEcc() const
{
	return ECC_Mine;
}

const int32& GameCharacterTeamInfo::GetEccFlag() const
{
	return ECCFlag_Mine;
}

const TArray<int>& GameCharacterTeamInfo::GetIdxesFriendly() const
{
	return Idxes_Team_Friendly;
}

const TArray<int>& GameCharacterTeamInfo::GetIdxesEnemy() const
{
	return Idxes_Team_Enemy;
}

int32 GameCharacterTeamInfo::GetEccFlagFriendly() const
{
	auto playerTeamInfo = AGameCharacter::AllyInfo->GetPlayerTeamInfo();
	if (playerTeamInfo && IsFriendly(playerTeamInfo->GetIdx()))
		return ECCFlag_Friendly | ECC_TO_BITFIELD(ECollisionChannel::ECC_GameTraceChannel1);
		
	return ECCFlag_Friendly;
}

int32 GameCharacterTeamInfo::GetEccFlagEnemy() const
{
	auto playerTeamInfo = AGameCharacter::AllyInfo->GetPlayerTeamInfo();
	if (playerTeamInfo && IsEnemy(playerTeamInfo->GetIdx()))
		return ECCFlag_Enemy | ECC_TO_BITFIELD(ECollisionChannel::ECC_GameTraceChannel1);
		
	return ECCFlag_Enemy;
}

bool GameCharacterTeamInfo::IsFriendly(const int& idx_Team) const
{
	return (IdxFlag_Friendly & (1 << idx_Team));
}

bool GameCharacterTeamInfo::IsEnemy(const int& idx_Team) const
{
	return (IdxFlag_Enemy & (1 << idx_Team));
}

ECollisionChannel GameCharacterTeamInfo::ConvertIdxToEcc(const int& idx_Team)
{
	switch(idx_Team)
	{
	case 0:		return ECC_GameTraceChannel2;
	case 1:		return ECC_GameTraceChannel3;
	case 2:		return ECC_GameTraceChannel4;
	default:	return ECC_Pawn;
	}
}

FString GameCharacterTeamInfo::ConvertIdxToCollisionProfileName(const int& idx_Team)
{
	switch(idx_Team)
	{
	case 0:		return TEXT("GameCharacter_1");
	case 1:		return TEXT("GameCharacter_2");
	case 2:		return TEXT("GameCharacter_3");
	default:	return TEXT("Pawn");
	}
}

inline GameCharacterContainer::GameCharacterContainer()
{
	int nMax = static_cast<int>(EGameCharacterStateTypes::None);
	m_llist_Container.SetNum(nMax);
}

inline GameCharacterContainer::~GameCharacterContainer()
{
	for (int i = 0 ; i < m_llist_Container.Num(); ++i)
		m_llist_Container[i].Empty(false);

	m_llist_Container.Empty();
}

TDoubleLinkedListEx<AGameCharacter*>* GameCharacterContainer::GetContainer(const EGameCharacterStateTypes& state)
{
	if (state == EGameCharacterStateTypes::None)
		return nullptr;
	
	int idx = static_cast<int>(state);
	return &m_llist_Container[idx];
}


GameCharacterAllyInfo::GameCharacterAllyInfo(const TArray<GameCharacterTeamInfo*>& teamInfos, int idx_PlayerTeam)
{
	m_map_TeamInfos.Empty();
	for (auto pTeamInfo : teamInfos)
	{
		if (!pTeamInfo
			|| pTeamInfo->GetIdx() < 0
			|| pTeamInfo->GetIdx() >= sc_MaxTeamCount)
				return;

		if (m_map_TeamInfos.Contains(pTeamInfo->GetIdx()))
			m_map_TeamInfos.Remove(pTeamInfo->GetIdx());

		m_map_TeamInfos.Add(pTeamInfo->GetIdx(), pTeamInfo);

		if (idx_PlayerTeam == pTeamInfo->GetIdx())
			m_TeamInfo_Player = pTeamInfo;

		// Init Container
		while (m_Container.Num() <= pTeamInfo->GetIdx())
			m_Container.Add(new GameCharacterContainer());	
	}
}

GameCharacterAllyInfo::~GameCharacterAllyInfo()
{
	m_TeamInfo_Player = nullptr;

	for(auto pair : m_map_TeamInfos)
		delete pair.Value;

	m_map_TeamInfos.Empty();

	for(auto pContainer  : m_Container)
		delete pContainer;
}

GameCharacterTeamInfo* GameCharacterAllyInfo::GetTeamInfo(const int& idx_Team)
{
	if (m_map_TeamInfos.Contains(idx_Team))
		return m_map_TeamInfos[idx_Team];

	return nullptr;
}

const GameCharacterTeamInfo* GameCharacterAllyInfo::GetPlayerTeamInfo() const
{
	return m_TeamInfo_Player;
}

void GameCharacterAllyInfo::SetPlayerTeamInfo(const GameCharacterTeamInfo* pTeamInfo_Player)
{
	m_TeamInfo_Player = pTeamInfo_Player;
}

TDoubleLinkedListEx<AGameCharacter*>* GameCharacterAllyInfo::GetTeamContainer(const int& idx_Team, const EGameCharacterStateTypes& state)
{
	if (idx_Team < 0 || sc_MaxTeamCount <= idx_Team)
		return nullptr;

	return m_Container[idx_Team]->GetContainer(state);
}

GameCharacterSpawnInfo::GameCharacterSpawnInfo()
{
	m_pSplineComp = nullptr;
}

void GameCharacterSpawnInfo::Init(const AGameCharacter* const& pOwner, const FVector& BeginPoint, const FQuat& BeginLook, const USplineComponent* const& splineComp)
{
	m_pOwner = pOwner;

	m_vBeginPoint = m_pOwner->CalNavLocation(BeginPoint);
	m_qBeginLook =  BeginLook, 
	m_pSplineComp = splineComp;

	ResetPatrolPoint();
}

const FVector& GameCharacterSpawnInfo::GetPatrolPoint()
{
	return m_vPatrolPoint;	
}

const FQuat& GameCharacterSpawnInfo::GetPatrolLook()
{
	return m_qPatrolLook;
}

void GameCharacterSpawnInfo::NextPatrolPoint(const float& WalkSpd)
{
	if (!IsValid(m_pSplineComp))
	{
		m_fCrrLength = 0.f;
		m_bBeginPoint = true;
		m_bEndPoint = true;
	}

	m_fCrrLength += WalkSpd;
	
	auto fSplineLength = m_pSplineComp->GetSplineLength();

	if (m_fCrrLength >= fSplineLength)
	{
		m_bBeginPoint = false;
		m_bEndPoint = true;
		m_fCrrLength = fSplineLength;
	}
	else if (m_fCrrLength <= 0.f)
	{
		m_bBeginPoint = true;
		m_bEndPoint = false;
		m_fCrrLength = 0.f;
	}
	else
	{
		m_bBeginPoint = false;
		m_bEndPoint = false;
	}

	m_vPatrolPoint = m_pOwner->CalNavLocation(m_pSplineComp->GetWorldLocationAtDistanceAlongSpline(m_fCrrLength));
	m_qPatrolLook = m_pSplineComp->GetWorldRotationAtDistanceAlongSpline(m_fCrrLength).Quaternion();
}

void GameCharacterSpawnInfo::ResetPatrolPoint()
{
	m_fCrrLength = 0.f;
	m_bBeginPoint = true;

	if (m_pSplineComp && m_pSplineComp->GetSplineLength() > 0.f)
	{
		m_vPatrolPoint = m_pOwner->CalNavLocation(m_pSplineComp->GetWorldLocationAtDistanceAlongSpline(0.f));
		m_qPatrolLook = m_pSplineComp->GetWorldRotationAtDistanceAlongSpline(0.f).Quaternion();
		m_bEndPoint = false;
	}
	else
	{
		m_vPatrolPoint = m_vBeginPoint;
		m_qPatrolLook = m_qBeginLook;
		m_bEndPoint = true;
	}
}

const bool& GameCharacterSpawnInfo::IsPatrolBeginPoint() const
{
	return m_bBeginPoint;
}

const bool& GameCharacterSpawnInfo::IsPatrolEndPoint() const
{
	return m_bEndPoint;
}

bool GameCharacterSpawnInfo::IsClosePatrolPoint(const FVector& actor, const float& fDist) const
{
	return (m_vPatrolPoint - actor).SizeSquared() < (fDist * fDist);
}

bool GameCharacterTargetInfo::IsValidTarget() const
{
	return m_Type != ETargetFindingResultTypes::None;
}

void GameCharacterTargetInfo::Clear()
{
	m_pTarget = nullptr;
	m_pBody = nullptr;
}

bool GameCharacterTargetInfo::LineTrace(FHitResult& result,  const FVector& vStart, const FVector& vEnd) const
{
	if (m_pBody)
		return m_pBody->LineTrace(result, vStart, vEnd, false);

	return false;
}

bool GameCharacterTargetInfo::Sweep(FHitResult& result, const FVector& vStart, const FVector& vEnd, const FQuat& qRot, const FCollisionShape& shape) const
{
	if (m_pBody)
		return m_pBody->Sweep(result, vStart, vEnd, qRot, shape, false);

	return false;
}		

FVector GameCharacterTargetInfo::GetLocation() const
{
	switch (m_Type)
	{
		case ETargetFindingResultTypes::Body:
			return m_pBody->GetUnrealWorldTransform().GetTranslation(); // same GetLocation
		case ETargetFindingResultTypes::Actor:
			return m_pTarget->GetActorLocation();
		case ETargetFindingResultTypes::Pos:
			return m_vLocation;
		default:
			break;
	}

	return FVector::ZeroVector;
}

FBodyInstance* HitResultToBodyInst(const FHitResult& hitResult)
{
	auto pChar = Cast<ACharacter>(hitResult.GetActor());

	// 캐릭터고 Bone에 맞은 경우
	if (pChar && hitResult.BoneName != NAME_None)
		return pChar->GetMesh()->GetBodyInstance(hitResult.BoneName);

	// 아닐 경우 Component에서 찾는다
	return hitResult.GetComponent()->GetBodyInstance();
}

FBodyInstance* HitResultToBodyInst(const FHitResult* const pHitResult)
{
	if (!pHitResult)
		return nullptr;

	return HitResultToBodyInst(*pHitResult);
}