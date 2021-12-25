// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameCharacter.h"
#include "IActionEventRuntime.h"

template<typename T>
class FRAMEWORK_API ActionEventRuntimeBase : public IActionEventRuntime
{
protected:
	const T* m_pData;
	UActionController* m_ActionCtrl;
	AGameCharacter* m_pOwner;

public:
	ActionEventRuntimeBase();
	virtual ~ActionEventRuntimeBase() override;

	virtual void Init(T& data, AGameCharacter* owner);
	virtual void InitLinkedActionRuntime(const TMap<FString, UActionRuntime*>& map_ActionRuntimes) override;

	virtual bool IsDuration() override;

	virtual float GetStartTime() override;
	virtual float GetEndTime() override;
	virtual bool GetEndOnActionEnd() override;
	virtual bool OnStart(float elapsedTime) override;
	virtual bool OnTick(float elapsedTime, float deltaTime) override;
	virtual void OnEnd(bool bIsForLoopReset) override;
	

	virtual TDoubleLinkedListEx<IActionEventRuntime*>::TDoubleLinkedListExNode* GetNode() override;
};

template<typename T>
ActionEventRuntimeBase<T>::ActionEventRuntimeBase()
{
	m_pOwner = nullptr;
	m_pData = nullptr;
	m_ActionCtrl = nullptr;
}

template<typename T>
ActionEventRuntimeBase<T>::~ActionEventRuntimeBase()
{
}

template<typename T>
void ActionEventRuntimeBase<T>::Init(T& data, AGameCharacter* owner)
{
	m_pData = &data;
	m_pOwner = owner;
	m_ActionCtrl = m_pOwner->GetActionCtrl();
}

template<typename T>
void ActionEventRuntimeBase<T>::InitLinkedActionRuntime(const TMap<FString, UActionRuntime*>& map_ActionRuntimes)
{
}

template<typename T>
bool ActionEventRuntimeBase<T>::IsDuration()
{
	return false;
}

template<typename T>
float ActionEventRuntimeBase<T>::GetStartTime()
{
	return m_pData->fStartTime;
}

template<typename T>
float ActionEventRuntimeBase<T>::GetEndTime()
{
	return 0.f;
}

template<typename T>
bool ActionEventRuntimeBase<T>::GetEndOnActionEnd()
{
	return false;
}

template<typename T>
bool ActionEventRuntimeBase<T>::OnStart(float elapsedTime)
{
	return false;
}

template<typename T>
bool ActionEventRuntimeBase<T>::OnTick(float elapsedTime, float deltaTime)
{
	return false;
}

template<typename T>
void ActionEventRuntimeBase<T>::OnEnd(bool bIsForLoopReset)
{
}

template<typename T>
TDoubleLinkedListEx<IActionEventRuntime*>::TDoubleLinkedListExNode* ActionEventRuntimeBase<T>::GetNode()
{
	return nullptr;
}
