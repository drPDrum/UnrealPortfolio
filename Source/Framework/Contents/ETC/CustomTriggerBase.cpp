#include "CustomTriggerBase.h"

ACustomTriggerBase::ACustomTriggerBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACustomTriggerBase::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(m_pShapeComp))
	{
		m_pShapeComp->OnComponentBeginOverlap.AddDynamic(this, &ACustomTriggerBase::OnTriggerBeginOverlap);
		m_pShapeComp->SetCollisionProfileName("PlayerTrigger");
		m_pShapeComp->SetEnableGravity(false);
	}
}

void ACustomTriggerBase::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnTrigger();
}

void ACustomTriggerBase::OnTrigger()
{
	
}
