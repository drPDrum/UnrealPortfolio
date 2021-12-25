#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_RotateInputU.h"
#include "ActionEventRuntime_RotateInputU.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_RotateInputU : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_RotateInputU>
{
	GENERATED_BODY()
public:

	UActionEventRuntime_RotateInputU();
	virtual ~UActionEventRuntime_RotateInputU() override;
	
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
};