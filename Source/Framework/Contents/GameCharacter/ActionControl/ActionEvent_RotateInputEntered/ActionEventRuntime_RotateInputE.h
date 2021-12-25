#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_RotateInputE.h"
#include "ActionEventRuntime_RotateInputE.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_RotateInputE : public UObject, public ActionEventDurationRuntimeBase<FActionEventData_RotateInputE>
{
	GENERATED_BODY()

private:	
	FQuat m_qDstWorldDir;
public:

	UActionEventRuntime_RotateInputE();
	virtual ~UActionEventRuntime_RotateInputE() override;
	
	virtual bool OnStart(float elapsedTime) override;
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
};