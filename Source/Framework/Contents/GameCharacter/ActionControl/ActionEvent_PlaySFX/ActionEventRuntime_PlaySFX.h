#pragma once

#include "../ActionEvent/ActionEventRuntimeBase.h"
#include "ActionEventData_PlaySFX.h"
#include "ActionEventRuntime_PlaySFX.generated.h"

class ASFXActor;

UCLASS()
class FRAMEWORK_API UActionEventRuntime_PlaySFX : public UObject, public ActionEventRuntimeBase<FActionEventData_PlaySFX>
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	USoundBase* m_pSoundBase;
	UPROPERTY()
	ASFXActor* m_pSFXActor;

public:
	UActionEventRuntime_PlaySFX();
	virtual ~UActionEventRuntime_PlaySFX() override;

	virtual void Init(FActionEventData_PlaySFX& data, AGameCharacter* owner) override;
	virtual bool OnStart(float elapsedTime) override;

	UFUNCTION()
	void OnReturnSFX(ASFXActor* pSFXReturned);
};