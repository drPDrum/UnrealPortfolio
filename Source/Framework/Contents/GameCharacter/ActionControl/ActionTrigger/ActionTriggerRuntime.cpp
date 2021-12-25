// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionTriggerRuntime.h"
#include "ActionTriggerData.h"

ActionTriggerRuntime::ActionTriggerRuntime(const FActionTriggerData& data, UActionRuntime* nextActionRuntime)
	: m_Data(data), m_NextActionRuntime(nextActionRuntime)
{
}

ActionTriggerRuntime::~ActionTriggerRuntime()
{
}

const FActionTriggerData& ActionTriggerRuntime::GetData()
{
	return m_Data;
}

UActionRuntime* ActionTriggerRuntime::GetNextActionRuntime()
{
	return m_NextActionRuntime;
}

bool ActionTriggerRuntime::HasNextActionRuntime()
{
	return m_NextActionRuntime != nullptr;
}

bool ActionTriggerRuntime::CheckTime(const float& fElapsedTime_Action, const float& fLength_Action) const
{
	if(m_Data.bSameAsActionEnd_StartTime)
		return fLength_Action <= fElapsedTime_Action;
	
	return m_Data.fStartTime <= fElapsedTime_Action && (m_Data.bSameAsActionEnd_EndTime || m_Data.fEndTime >= fElapsedTime_Action);
}
