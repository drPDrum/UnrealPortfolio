// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionData.h"
#include "ActionRuntime.generated.h"

class IActionEventRuntime;
class ActionTriggerRuntime;

UCLASS()
class FRAMEWORK_API UActionRuntime : public UObject
{
	GENERATED_BODY()
private:
	UActionData* m_Data;
	int refCount;

	UPROPERTY()
	TArray<UObject*> m_EventRuntimeContainer; // 가비지 컬렉팅 방지
	TArray<IActionEventRuntime*> m_EventRuntimes;

	TArray<TArray<ActionTriggerRuntime*>> m_TriggerRuntimeses;
	TMap<FString, ActionTriggerRuntime*> m_set_TriggerRuntimesByActionName;

	bool m_bResetAI;

public:
	UActionRuntime();
	virtual ~UActionRuntime() override;

	void Init(UActionData* data, AGameCharacter* owner);
	const UActionData* GetData() const;

	int GetRefCount();
	void AddRefCount();
	void SubRefCount();

	IActionEventRuntime* GetEventRuntime(int idx);
	int GetEventRuntimeSize();

	void InitTriggers(const TMap<FString, UActionRuntime*>& map_ActionRuntimes);
	const TArray<ActionTriggerRuntime*>& GetTriggerRuntimes(ETriggerEventTypes eventType);
	const ActionTriggerRuntime* GetTriggerRuntime(FString actionName) const;
	bool HasNextAction(const FString& actionName) const;

	const bool& IsResetAI() const;
};