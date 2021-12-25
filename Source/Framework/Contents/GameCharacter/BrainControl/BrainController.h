#pragma once

#include "AIController.h"
#include "../GameCharacter_Define.h"
#include "BrainController.generated.h"

class CCharacterData;
class USurveyInfo;
class AGameCharacter;
class USplineComponent;


UCLASS()
class FRAMEWORK_API ABrainController : public AAIController
{
	GENERATED_BODY()

protected: // For All
	UPROPERTY()
	AGameCharacter* m_pOwner;
	const CCharacterData* m_pOwnerData;

	// 현재 타겟 정보
	GameCharacterTargetInfo m_TargetInfo;

	// 계산 완료 값	
	FQuat		m_qLookWorldDir;	// 카메라 또는 AI가 보고있는 월드 방향
	FQuat		m_qInputWorldDir;	// UserInput 또는 AI가 입력하고 있는 월드 방향
	FVector		m_vInputWorldDir;	// UserInput 또는 AI가 입력하고 있는 월드 방향

protected: // For AI

	
	EAIDefaultBehaviorType m_DefaultAIBehaviorType;
	EAIBehaviorTypes m_CurrAIBehaviorType;

	GameCharacterSpawnInfo* m_pSpawnInfo; // BTTask_Patrol을 사용하고 싶다면 세팅한다.
	
	UPROPERTY()
	USurveyInfo* m_SurveyInfo;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* m_AIBT;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBlackboardData* m_AIBB;
	

	bool m_bIsAIInCombo;

protected: // For Input
	// 유저의 컨트롤하에 있는지
	bool m_IsOrderByUserInput;
	
	// UserInput으로 부터 온 값을 임시 저장
	FVector2D m_vInputScaleMove;
	// 입력된 방향 종류
	ETriggerAxisTypes m_AxisTriggerType;
	// 실제 Input Scale
	float m_fInputScale;

	bool m_bRefreshInput;

public: // For All
	ABrainController();
	virtual ~ABrainController() override;

	void Init(AGameCharacter* pOwner);
	AGameCharacter* GetOwner();

	void OnOwnerTick(float deltaTime);
	void RefreshInputDir(bool bForced);

	const FQuat& GetAxisWorldDirQ();
	const FVector& GetAxisWorldDirV();
	const ETriggerAxisTypes& GetAxisTriggerType();
	const float& GetAxisPower();

	void SetLookInfo(FQuat qLookWorldDir);
	const FQuat& GetLookInfoQ() const;
	const FVector& GetLookInfoV() const;

	bool SetTarget(const float& radius, const EGameCharacterFindTargetTypes& targetType, const EGameCharacterStateTypes& stateType);
	void SetTarget(const FHitResult& hitResult);
	void SetTarget(AActor* pTarget, FBodyInstance* pBody);
	void SetTarget(const FVector& vTargetLocation);
	
	const GameCharacterTargetInfo& GetTargetInfo() const;

public: // For AI
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void StartAI();
	void StopAI();
	void ResetAI();

public: // For AI :: BT 전용
	void SetSurveyInfo();
	USurveyInfo* GetSurveyInfo();

	void SetDefaultAIBehavior(const EAIDefaultBehaviorType& behaviorType);
	const EAIDefaultBehaviorType& GetDefaultAIBehavior() const;
	void SetCurrAIBehavior(const EAIBehaviorTypes& behaviorType);
	const EAIBehaviorTypes& GetCurrAIBehavior() const;

	const bool& IsAIInCombo() const;
	void SetAIInCombo(const bool& bCombo);

	void SetSpawnInfo(const FVector& vStartPoint, const FQuat& qStartLook, const USplineComponent* const& pSplineComp);
	GameCharacterSpawnInfo* const& GetSpawnInfo() const;
	void ClearSpawnInfo();

public:	// For Input
	void SetOrderByUserInput(const bool& bEnable);
	const bool& GetOrderByUserInput() const;
	void OnInput_MoveAxisX(const float& scale);
	void OnInput_MoveAxisY(const float& scale);
	void OnInput_Key(const ETriggerEventTypes& keyEvent);
};