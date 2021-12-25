// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ActionEventRuntimeBase.h"

template<typename T>
class FRAMEWORK_API ActionEventDurationRuntimeBase : public ActionEventRuntimeBase<T>
{
protected:
	TDoubleLinkedListEx<IActionEventRuntime*>::TDoubleLinkedListExNode* m_LlistNode;

public:
	ActionEventDurationRuntimeBase();
	virtual ~ActionEventDurationRuntimeBase() override;

	virtual bool IsDuration() override;
	virtual float GetEndTime() override;
	virtual bool GetEndOnActionEnd() override;

	virtual TDoubleLinkedListEx<IActionEventRuntime*>::TDoubleLinkedListExNode* GetNode() override;
};

template<typename T>
ActionEventDurationRuntimeBase<T>::ActionEventDurationRuntimeBase()
{
	m_LlistNode = new TDoubleLinkedListEx<IActionEventRuntime*>::TDoubleLinkedListExNode(this);
}

template<typename T>
ActionEventDurationRuntimeBase<T>::~ActionEventDurationRuntimeBase()
{
	m_LlistNode->RemoveSelf();
	delete m_LlistNode;
}

template<typename T>
bool ActionEventDurationRuntimeBase<T>::IsDuration()
{
	return true;
}

template<typename T>
float ActionEventDurationRuntimeBase<T>::GetEndTime()
{
	return m_pData->fEndTime;
}

template<typename T>
bool ActionEventDurationRuntimeBase<T>::GetEndOnActionEnd()
{
	return m_pData->bSameAsActionEnd;
}

template<typename T>
TDoubleLinkedListEx<IActionEventRuntime*>::TDoubleLinkedListExNode* ActionEventDurationRuntimeBase<T>::GetNode()
{
	return m_LlistNode;
}

