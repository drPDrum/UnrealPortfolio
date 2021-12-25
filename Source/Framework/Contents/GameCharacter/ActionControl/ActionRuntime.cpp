// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionRuntime.h"
#include "ActionEvent/IActionEventRuntime.h"
#include "ActionTrigger/ActionTriggerRuntime.h"

UActionRuntime::UActionRuntime()
{
	m_Data = nullptr;
	refCount = 0;
}

UActionRuntime::~UActionRuntime()
{
	m_EventRuntimeContainer.Empty();
	m_EventRuntimes.Empty();
	
	// Trigger Runtime 삭제
	for (auto triggerRuntimes : m_TriggerRuntimeses)
	{
		for (auto pTriggerRuntime : triggerRuntimes)
			delete pTriggerRuntime;
	}
}

void UActionRuntime::Init(UActionData* data, AGameCharacter* owner)
{
	m_Data = data;
	refCount = 0;

	// Event Runtime 생성
	auto eventDatas = data->EventDatas;
	for (auto eventData : eventDatas)
	{
		auto newRuntimeInfo = eventData->CreateRuntime(owner);
		m_EventRuntimeContainer.Add(newRuntimeInfo.pUObject);
		m_EventRuntimes.Add(newRuntimeInfo.pRuntime);
	}

	// 이 액션 실행시 AI가 리셋되어야 하는가?
	switch (m_Data->actionType)
	{
		case EActionTypes::Idle:
		case EActionTypes::HitReaction:
			m_bResetAI = true;
			break;
		default:
			m_bResetAI = false;
			break;
	}
}

const UActionData* UActionRuntime::GetData() const
{
	return m_Data;
}

int UActionRuntime::GetRefCount()
{
	return refCount;
}

void UActionRuntime::AddRefCount()
{
	++refCount;
}

void UActionRuntime::SubRefCount()
{
	--refCount;
}


IActionEventRuntime* UActionRuntime::GetEventRuntime(int idx)
{
	return m_EventRuntimes[idx];
}

int UActionRuntime::GetEventRuntimeSize()
{
	return m_EventRuntimes.Num();
}

const TArray<ActionTriggerRuntime*>& UActionRuntime::GetTriggerRuntimes(ETriggerEventTypes eventType)
{
	return m_TriggerRuntimeses[static_cast<int>(eventType)];
}

const ActionTriggerRuntime* UActionRuntime::GetTriggerRuntime(FString actionName) const
{
	if (m_set_TriggerRuntimesByActionName.Contains(actionName))
		return m_set_TriggerRuntimesByActionName[actionName];
	
	return nullptr;
}

bool UActionRuntime::HasNextAction(const FString& actionName) const
{
	return m_set_TriggerRuntimesByActionName.Contains(actionName);
}

const bool& UActionRuntime::IsResetAI() const
{
	return m_bResetAI;
}

void UActionRuntime::InitTriggers(const TMap<FString, UActionRuntime*>& map_ActionRuntimes)
{
	// Trigger 정리
	auto nEventTriggerTypeCount = static_cast<int>(ETriggerEventTypes::Max);
	for (int i = 0 ; i < nEventTriggerTypeCount; ++i)
		m_TriggerRuntimeses.Add(TArray<ActionTriggerRuntime*>());
	
	auto& triggerDatas = m_Data->TriggerDatas;
	for (auto& triggerData : triggerDatas)
	{
		if (map_ActionRuntimes.Contains(triggerData.NextActionName))
		{
			auto pNextActionRuntime = map_ActionRuntimes[triggerData.NextActionName];
			auto pTriggerRuntime = new ActionTriggerRuntime(triggerData, pNextActionRuntime);
			
			m_TriggerRuntimeses[static_cast<int>(triggerData.EventType)].Add(pTriggerRuntime);	
			m_set_TriggerRuntimesByActionName.Add(pTriggerRuntime->GetData().NextActionName, pTriggerRuntime);
		}
		else
		{
			LOG_ERROR(TEXT("ActionData is Not Found %s"), *triggerData.NextActionName)
		}
	}

	// Event에서 직접 링크된 액션 정리
	for (auto eventRuntime : m_EventRuntimes)
		eventRuntime->InitLinkedActionRuntime(map_ActionRuntimes);
}


