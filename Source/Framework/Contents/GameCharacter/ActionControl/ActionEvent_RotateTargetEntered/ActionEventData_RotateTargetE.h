#pragma once

#include "../ActionEvent/ActionEventDurationDataBase.h"
#include "ActionEventData_RotateTargetE.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_RotateTargetE: public FActionEventDurationDataBase
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere)
	float m_fRotateSpdPerSec;

public:
	FActionEventData_RotateTargetE();
	virtual ~FActionEventData_RotateTargetE() override;
	
	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;

	virtual ActionEventTypes GetActionEventType() const override;
};
