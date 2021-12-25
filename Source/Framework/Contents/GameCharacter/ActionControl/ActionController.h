// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameCharacter.h"
#include "ActionController.generated.h"

class UActionData;
class UActionRuntime;
class IActionEventRuntime;

class AFXActor;
class ASFXActor;

UCLASS()
class FRAMEWORK_API UActionController : public UObject
{
	GENERATED_BODY()
	
private:
	AGameCharacter* m_pOwner;

	UPROPERTY()
	TMap<UActionData*, UActionRuntime*> m_map_ActionRuntime;
	UPROPERTY()
	TMap<FString, UActionRuntime*> m_map_ActionRuntimeByName;

	TSet<AFXActor*> m_set_LoopingFX;
	TSet<ASFXActor*> m_set_LoopingSFX;
	
	UActionRuntime* m_pCrrActionRuntime;

	float m_fElapsedTime;

	TDoubleLinkedListEx<IActionEventRuntime*> m_llist_EventRuntimesForUpdate;
	int m_Idx_EventRuntime;

public:
	UActionController();
	virtual ~UActionController() override;

	void Init(AGameCharacter* owner);
	AGameCharacter* GetOwner();

private:
	bool CheckStartEvent();
	void OnEndAction();

public:
	void OnOwnerTick(float deltaTime);

	void AddActionData(UActionData* actionData);
	void InitActionDatas(const TArray<UActionData*>& actionDatas);

	void PlayAction(UActionRuntime* actionRuntime);
	void PlayAction(const FString& actionName);
	void PlayAction(UActionData* actionData);
	void StopAction();

	bool IsPlaying() const;
	bool IsPlaying(const FString& actionDataName) const;

	const UActionRuntime* GetCrrActionRuntime() const;
	const float& GetActionElapsedTime() const;
	UActionRuntime* CheckNextAction(ETriggerEventTypes eventType);

	void AddLoopingFX(AFXActor* pFxActor);
	void RemoveLoopingFX(AFXActor* pFxActor);
	void AddLoopingSFX(ASFXActor* pSfxActor);
	void RemoveLoopingSFX(ASFXActor* pSfxActor);
};