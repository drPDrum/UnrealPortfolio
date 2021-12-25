#pragma once

#include "Framework/CommonInfo.h"
#include "CustomTriggerBase.generated.h"


UCLASS(Abstract)
class FRAMEWORK_API ACustomTriggerBase : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UShapeComponent* m_pShapeComp;
	
public:	
	// Sets default values for this actor's properties
	ACustomTriggerBase();

	protected:
	virtual void BeginPlay() override;

	public:	
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void OnTrigger();
};