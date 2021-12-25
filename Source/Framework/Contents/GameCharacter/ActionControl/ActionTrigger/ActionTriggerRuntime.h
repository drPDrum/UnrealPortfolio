// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class FRAMEWORK_API ActionTriggerRuntime
{
	const struct FActionTriggerData& m_Data;
	class UActionRuntime* m_NextActionRuntime;
	
public:
	ActionTriggerRuntime(const FActionTriggerData& data, UActionRuntime* nextActionRuntime);
	~ActionTriggerRuntime();


	const FActionTriggerData& GetData();
	UActionRuntime* GetNextActionRuntime();
	bool HasNextActionRuntime();
	bool CheckTime(const float& fElapsedTime_Action, const float& fLength_Action) const;
};
