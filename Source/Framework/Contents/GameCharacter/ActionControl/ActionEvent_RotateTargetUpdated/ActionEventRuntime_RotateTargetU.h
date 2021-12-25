#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_RotateTargetU.h"
#include "ActionEventRuntime_RotateTargetU.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_RotateTargetU : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_RotateTargetU>
{
	GENERATED_BODY()
public:
	
	UActionEventRuntime_RotateTargetU();
	virtual ~UActionEventRuntime_RotateTargetU() override;
	
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
};