#pragma once

#include "../ActionEvent/ActionEventDurationDataBase.h"
#include "ActionEventData_RotateTargetU.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_RotateTargetU : public FActionEventDurationDataBase
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere)
	float m_fRotateSpdPerSec;

public:
	FActionEventData_RotateTargetU();
	virtual ~FActionEventData_RotateTargetU() override;
	
	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;

	virtual ActionEventTypes GetActionEventType() const override;
};