#pragma once

#include "../ActionEvent/ActionEventDurationDataBase.h"
#include "ActionEventData_RotateInputE.generated.h"

USTRUCT()
struct FRAMEWORK_API FActionEventData_RotateInputE: public FActionEventDurationDataBase
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere)
	float m_fRotateSpdPerSec;

public:
	FActionEventData_RotateInputE();
	virtual ~FActionEventData_RotateInputE() override;
	
	virtual CreateRuntimeResult CreateRuntime(AGameCharacter* owner) override;

	virtual ActionEventTypes GetActionEventType() const override;
};
