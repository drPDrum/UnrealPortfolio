#include "SpawnPoint.h"
#include "Components/SplineComponent.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(m_pRoot);
	
	m_pSplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	m_pSplineComp->SetupAttachment(m_pRoot);
	
	m_pPatrolPoint = nullptr;
	m_pSpawnedCharacter = nullptr;
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	if (HasPatrol())
	{
		auto& tr = GetTransform();

		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_pPatrolPoint = GetWorld()->SpawnActor<AActor>(tr.GetLocation(), tr.Rotator(), param);
		
#if WITH_EDITOR
		m_pPatrolPoint->SetActorLabel(FString::Printf(TEXT("PatrolPoint_%s"), *this->GetActorLabel()));
#endif
		m_pPatrolPoint->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

const FString& ASpawnPoint::GetLandName() const
{
	return m_strLandLevelName;
}

const ESpawnObjectType& ASpawnPoint::GetSpawnObjectType() const
{
	return m_eSpawnType;
}

const EWorldId& ASpawnPoint::GetPrevWorldID() const
{
	return m_ePrevWorldID;
}