#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_RotateTargetE.h"
#include "ActionEventRuntime_RotateTargetE.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_RotateTargetE : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_RotateTargetE>
{
	GENERATED_BODY()

private:	
	FQuat m_qDstWorldDir;

public:
	
	UActionEventRuntime_RotateTargetE();
	virtual ~UActionEventRuntime_RotateTargetE() override;
	
	virtual bool OnStart(float elapsedTime) override;
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
};
