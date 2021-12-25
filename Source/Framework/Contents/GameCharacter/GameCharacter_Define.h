#pragma once

#include "../../CommonInfo.h"
#include "RuntimeUtils/GenericEx/DoubleLinkedListEx.h"

class AGameCharacter;
class USplineComponent;

UENUM()
enum class EActionTypes : uint32
{
	Idle,
	HitReaction,
	Move,
	Attack,
};

UENUM()
enum class ETriggerAxisTypes : uint8
{
	// 입력하지 않아야 함
	None,
	// 윗방향
	Front,
	// 옆방향
	Side,
	// 아랫방향
	Back,
	
	// 아무거나 입력해도 됨 (입력은 해야함)
	Any,
	// 체크 하지 않음
	NoUse,	
};

// Index로 사용중
UENUM()
enum class ETriggerEventTypes : uint8
{
	None = 0,
	Attack00_Down,
	Attack00_Up,
	Attack01_Down,
	Attack01_Up,
	Jump_Down,
	Jump_Up,
	Dash_Down,
	Dash_Up,
	OnGround,
	OnAir,
	Max,
};

// 인덱스로 사용중, 값변경 금지
UENUM()
enum class EGameCharacterStateTypes : uint8
{
	Alive,
	Die,
	None,
};

UENUM()
enum class EGameCharacterFindTargetTypes : uint8
{
	Friendly,
	Enemy,
	Both
};

UENUM()
enum class EGameCharacterAreaTyeps : uint8
{
	Ground,
	Air,
	Water,
};

// 인덱스로 사용 중, 값 변경 X 
UENUM()
enum class EStatTypes : uint8
{
	MaxHP,
	ATK,

	MAX,
};

/* EAIDefaultBehaviorType와 같은 벨류를 맞출것 */
UENUM()
enum class EAIBehaviorTypes : uint8
{
	Battle = 0,

	None = 1,
	PatrolLoop = 2,
	PatrolPingPong = 3,
};

/* EAIBehaviorType와 같은 벨류를 맞출것 */
UENUM()
enum class EAIDefaultBehaviorType : uint8
{
	None = 1,
	PatrolLoop = 2,
	PatrolPingPong = 3,
};

class GameCharacterTeamInfo
{
private:
	int Idx_Mine;
	FString CollisionProfile_Mine;
	ECollisionChannel ECC_Mine;
	int32 ECCFlag_Mine;
	
	TArray<int> Idxes_Team_Friendly;		// 우리팀이 아군으로 판단하는 팀 Idx
	TArray<int> Idxes_Team_Enemy;			// 우리팀이 적군으로 판단하는 팀 Idx

	int32 ECCFlag_Friendly;
	int32 ECCFlag_Enemy;

	int IdxFlag_Friendly;
	int IdxFlag_Enemy;
public:
	GameCharacterTeamInfo(const int& idx_Mine, TArray<int> idxes_Friendly, TArray<int> idxes_Enemy);
	~GameCharacterTeamInfo();

	const int& GetIdx() const;
	const FString& GetCollisionProfile() const;
	const ECollisionChannel& GetEcc() const;
	const int32& GetEccFlag() const;

	const TArray<int>& GetIdxesFriendly() const;
	const TArray<int>& GetIdxesEnemy() const;

	int32 GetEccFlagFriendly() const;
	int32 GetEccFlagEnemy() const;

	bool IsFriendly(const int& idx_Team) const;
	bool IsEnemy(const int& idx_Team) const;

	static ECollisionChannel ConvertIdxToEcc(const int& idx_Team);
	static FString ConvertIdxToCollisionProfileName(const int& idx_Team);
};

class GameCharacterContainer
{
	TArray<TDoubleLinkedListEx<AGameCharacter*>> m_llist_Container;
public :
	GameCharacterContainer();
	~GameCharacterContainer();

	TDoubleLinkedListEx<AGameCharacter*>* GetContainer(const EGameCharacterStateTypes& state);
};

class GameCharacterAllyInfo
{
	static const int sc_MaxTeamCount = 6;
	const GameCharacterTeamInfo* m_TeamInfo_Player = nullptr;
	TMap<int, GameCharacterTeamInfo*> m_map_TeamInfos;
	TArray<GameCharacterContainer*> m_Container;

public:
	GameCharacterAllyInfo(const TArray<GameCharacterTeamInfo*>& teamInfos, int idx_PlayerTeam);
	~GameCharacterAllyInfo();
	
	GameCharacterTeamInfo* GetTeamInfo(const int& idx_Team);
	const GameCharacterTeamInfo* GetPlayerTeamInfo() const;
	void SetPlayerTeamInfo(const GameCharacterTeamInfo* pTeamInfo_Player);
	TDoubleLinkedListEx<AGameCharacter*>* GetTeamContainer(const int& idx_Team, const EGameCharacterStateTypes& state);
};

class GameCharacterSpawnInfo
{
	const AGameCharacter* m_pOwner;
	const USplineComponent* m_pSplineComp;

	FVector m_vBeginPoint;
	FQuat m_qBeginLook;

	FVector m_vPatrolPoint;
	FQuat m_qPatrolLook;

	bool m_bBeginPoint;
	bool m_bEndPoint;

	float m_fCrrLength;

public:
	GameCharacterSpawnInfo();

	void Init(const AGameCharacter* const& pOwner, const FVector& BeginPoint, const FQuat& BeginLook, const USplineComponent* const& splineComp);

	const FVector& GetPatrolPoint();
	const FQuat& GetPatrolLook();
	void NextPatrolPoint(const float& WalkSpd);
	void ResetPatrolPoint();

	const bool& IsPatrolBeginPoint() const;
	const bool& IsPatrolEndPoint() const;
	bool IsClosePatrolPoint(const FVector& actor, const float& fDist) const;
};

UENUM()
enum class ETargetFindingResultTypes : uint8
{
	None,
	Pos,
	Actor,
	Body,
};

struct GameCharacterTargetInfo
{
	ETargetFindingResultTypes m_Type;
	AActor* m_pTarget;
	const FBodyInstance* m_pBody;
	FVector m_vLocation;

	bool IsValidTarget() const;
	void Clear();
	bool LineTrace(FHitResult& result, const FVector& vStart, const FVector& vEnd) const;
	bool Sweep(FHitResult& result, const FVector& vStart, const FVector& vEnd, const FQuat& qRot, const FCollisionShape& shape) const;
	FVector GetLocation() const;
};

FBodyInstance* HitResultToBodyInst(const FHitResult& hitResult);
FBodyInstance* HitResultToBodyInst(const FHitResult* const pHitResult);