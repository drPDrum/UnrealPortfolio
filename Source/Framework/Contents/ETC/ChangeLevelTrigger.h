#pragma once
#include "CustomTriggerBase.h"
#include "ChangeLevelTrigger.generated.h"

enum class EWorldId : uint8;

UCLASS()
class FRAMEWORK_API AChangeLevelTrigger : public ACustomTriggerBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EWorldId m_eLoadWorldID;
	
public:	
	// Sets default values for this actor's properties
	AChangeLevelTrigger();

public:	
	virtual void OnTrigger() override;
};