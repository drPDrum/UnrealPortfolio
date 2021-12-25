#pragma once
#include "Framework/CommonInfo.h"
#include "Framework/Managers/WorldManager/WorldManager.h"
#include "SpawnPoint.generated.h"

class USplineComponent;
class AGameCharacter;
enum class EWorldId : uint8;
enum class EAIDefaultBehaviorType : uint8;

UENUM()
enum class ESpawnObjectType : uint8
{
	None,
	Player,
	Monster,
	Object
};

UCLASS()
class FRAMEWORK_API ASpawnPoint : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USceneComponent* m_pRoot;
	UPROPERTY(EditAnywhere, Category = "Spawn|Common Info", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString m_strLandLevelName;
	UPROPERTY(EditAnywhere, Category="Spawn|Common Info", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ESpawnObjectType m_eSpawnType;
	UPROPERTY(EditAnywhere, Category="Spawn|Common Info", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EWorldId m_ePrevWorldID;
	UPROPERTY(EditAnywhere, Category="Spawn|Monster Info", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 m_nMonsterCharDataID;
	/* 패트롤용 변수들 */
	UPROPERTY(EditAnywhere, Category="Spawn|Monster Info", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EAIDefaultBehaviorType m_ePatrolType;
	UPROPERTY(EditAnywhere, Category="Spawn|Monster Info", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USplineComponent* m_pSplineComp;

	/* 패트롤 런타임용 변수들 */
	UPROPERTY()
	AActor* m_pPatrolPoint;
	float m_fCrrPatolPointLength;

	AGameCharacter* m_pSpawnedCharacter;
	bool m_bCharacterActive;
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	const FString& GetLandName() const;
	const ESpawnObjectType& GetSpawnObjectType() const;
	const EWorldId& GetPrevWorldID() const;
	
private:
	bool HasPatrol() const;

	// +Character.cpp
public:
	AGameCharacter* SpawnCharacter();
	void UnSpawnCharacter();
	void SetActive_Character(bool bEnable, bool bControlAI = true);
	bool IsActive_Character() const;
	
	void OnStart_Character();		
};