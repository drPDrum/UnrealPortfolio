#include "FrameworkCamera.h"

// Sets default values
AFrameworkCamera::AFrameworkCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Component
	m_Body = CreateDefaultSubobject<USceneComponent>(TEXT("Body"));
	SetRootComponent(m_Body);

	m_Neck = CreateDefaultSubobject<USceneComponent>(TEXT("Neck"));
	m_Neck->SetupAttachment(m_Body);

	m_Head = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));
	m_Head->SetupAttachment(m_Neck);

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Camera->SetupAttachment(m_Head);

	// Set Default Value;
	m_TargetActor = nullptr;

	m_fMinLength = 10.f;
	m_fMaxLength = 100.f;

	m_fYawDegreePerSec = 360;
	m_fPitchDegreePerSec = 360;


	m_vOffsetToTarget.X = 400.f;
	m_vOffsetToTarget.Y = 0.f;
	m_vOffsetToTarget.Z = 50.f;

	m_bNeedUpdateRotation = false;
	m_bUseZoomPerTargetVelocity = true;
}

// Called when the game starts or when spawned
void AFrameworkCamera::BeginPlay()
{
	Super::BeginPlay();

	m_fYawDegree = GetActorRotation().Yaw;
	
	m_Neck->SetRelativeLocation(FVector(0.f, m_vOffsetToTarget.Y, m_vOffsetToTarget.Z));
	m_Head->TargetArmLength = m_vOffsetToTarget.X;
}

// Called every frame
void AFrameworkCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Follow Target
	if (m_TargetActor != nullptr)
	{
		if(m_bUseZoomPerTargetVelocity)
		{
			float fVelocity = m_TargetActor->GetVelocity().Size();
			float fTargetArmLength = 0.f;
			float fCurLength = m_Head->TargetArmLength;
			
			if(fVelocity > 700.f)
				fTargetArmLength = m_vOffsetToTarget.X + fVelocity * 0.2f;
			else
				fTargetArmLength = m_vOffsetToTarget.X;
		
			if(fTargetArmLength != fCurLength)
			{
				float fMoveDelta = fTargetArmLength - fCurLength;
				fMoveDelta *= DeltaTime;
				fCurLength += fMoveDelta;
		
				if(fCurLength <= m_vOffsetToTarget.X)
					fCurLength = m_vOffsetToTarget.X;
		
				m_Head->TargetArmLength = fCurLength;
			}
		}
		
		SetActorLocation(m_TargetActor->GetActorLocation());
	}
}

void AFrameworkCamera::SetTarget(AActor* targetActor)
{
	m_TargetActor = targetActor;
}

UCameraComponent* AFrameworkCamera::GetCamera()
{
	return m_Camera;
}


void AFrameworkCamera::OnInput_AxisLookX(float scale)
{
	if (scale == 0.f)
		return;

	m_fYawDegree += scale * m_fYawDegreePerSec * GetWorld()->DeltaTimeSeconds;
	if (m_fYawDegree < 0.f)
		m_fYawDegree += 360.f;
	else if (m_fYawDegree > 360.f)
		m_fYawDegree -= 360.f;

	if (m_TargetActor)
		SetActorRotation(FQuat(m_TargetActor->GetActorUpVector(), FMath::DegreesToRadians(m_fYawDegree)).Rotator());
	else
		SetActorRotation(FQuat(GetActorUpVector(), FMath::DegreesToRadians(m_fYawDegree)).Rotator());
}

void AFrameworkCamera::OnInput_AxisLookY(float scale)
{
	if (scale == 0.f)
		return;

	m_fPitchDegree += scale * m_fPitchDegreePerSec * GetWorld()->DeltaTimeSeconds;
	m_fPitchDegree = FMath::Clamp(m_fPitchDegree, -89.f, 89.f);

	m_Neck->SetRelativeRotation(FRotator(m_fPitchDegree, 0.f, 0.f));
}