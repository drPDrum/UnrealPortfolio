#pragma once

#include "../ActionEvent/ActionEventDurationRuntimeBase.h"
#include "ActionEventData_Rotate.h"
#include "ActionEventRuntime_Rotate.generated.h"

UCLASS()
class FRAMEWORK_API UActionEventRuntime_Rotate :public UObject, public ActionEventDurationRuntimeBase<FActionEventData_Rotate>
{
	GENERATED_BODY()
	
private:
	float m_fRotateRadianSpdPerSec;

public:
	UActionEventRuntime_Rotate();
	virtual ~UActionEventRuntime_Rotate() override;

	virtual void Init(FActionEventData_Rotate& data, AGameCharacter* owner) override;
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
};