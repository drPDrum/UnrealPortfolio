// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionController.h"
#include "ActionRuntime.h"
#include "ActionEvent/ActionEventRuntimeBase.h"
#include "ActionTrigger/ActionTriggerData.h"
#include "ActionTrigger/ActionTriggerRuntime.h"
#include "../BrainControl/BrainController.h"
#include "Framework/Managers/FXManager/FXActor.h"
#include "Framework/Managers/SFXManager/SFXActor.h"

UActionController::UActionController()
{
	m_pOwner = nullptr;
	m_pCrrActionRuntime = nullptr;
	m_fElapsedTime = 0.f;
	m_Idx_EventRuntime = 0;
}

UActionController::~UActionController()
{
	m_pCrrActionRuntime = nullptr;

	m_map_ActionRuntime.Empty();
	m_map_ActionRuntimeByName.Empty();
	m_llist_EventRuntimesForUpdate.Empty(false);
	
	m_pOwner = nullptr;
}

void UActionController::Init(AGameCharacter* owner)
{
	m_pOwner = owner;
}

AGameCharacter* UActionController::GetOwner()
{
	return m_pOwner;
}

void UActionController::OnOwnerTick(float deltaTime)
{
	if (m_pCrrActionRuntime == nullptr)
		return;

	m_fElapsedTime += deltaTime;

	// Update Event Runtime
	{
		auto pNode_Event = m_llist_EventRuntimesForUpdate.GetHead();
		while (pNode_Event)
		{
			auto pCrrNode_Event = pNode_Event;
			pNode_Event = pNode_Event->GetNextNode();
		
			auto pEventRuntime = pCrrNode_Event->GetValue();
			if (pEventRuntime->OnTick(m_fElapsedTime -  pEventRuntime->GetStartTime(), deltaTime))
				return;

			// 시작하자 마자 끝났다면 eventRuntime 종료
			if (!pEventRuntime->GetEndOnActionEnd() && pEventRuntime->GetEndTime() <= m_fElapsedTime)
			{
				pEventRuntime->OnEnd(false);
				m_llist_EventRuntimesForUpdate.RemoveNode(pCrrNode_Event, false);
			}
		}
	}

	// Check Start
	if (CheckStartEvent())
		return;

	auto fCrrActionLength = m_pCrrActionRuntime->GetData()->fLength;
	bool bIsCrrActionLastTick = m_fElapsedTime >= fCrrActionLength;

	// 마지막 프레임일 때
	if (bIsCrrActionLastTick)
	{
		// 루프 형이면
		if (m_pCrrActionRuntime->GetData()->bIsLoop)
		{
			m_fElapsedTime = fmod(m_fElapsedTime, fCrrActionLength);
			m_Idx_EventRuntime = 0;

			// 진행중인 모든 EventRuntime 종료
			auto pNode_Event = m_llist_EventRuntimesForUpdate.GetHead();
			while (pNode_Event)
			{
				auto pCrrNode_Event = pNode_Event;
				pNode_Event = pNode_Event->GetNextNode();

				pCrrNode_Event->GetValue()->OnEnd(true);
				m_llist_EventRuntimesForUpdate.RemoveNode(pCrrNode_Event, false);
			}
		}
		// 아니면 액션 종료
		else
		{
			// AI라면 Input 정리
			const auto pBrainCtrl = m_pOwner->GetBrainCtrl();
			if (!pBrainCtrl->GetOrderByUserInput())
			{
				pBrainCtrl->OnInput_MoveAxisX(0.f);
				pBrainCtrl->OnInput_MoveAxisY(0.f);
				pBrainCtrl->RefreshInputDir(false);
			}
			
			// Check Next Action :: None
			const auto& pNextActionRuntime = CheckNextAction(ETriggerEventTypes::None);

			// 다음 액션이 있다면
			if (pNextActionRuntime)
				PlayAction(pNextActionRuntime);
			else
				OnEndAction();
		}
	}
	else
	{
		// Check Next Action :: None
		PlayAction(CheckNextAction(ETriggerEventTypes::None));
	}
}

bool UActionController::CheckStartEvent()
{
	while (m_Idx_EventRuntime < m_pCrrActionRuntime->GetEventRuntimeSize())
	{
		auto pEventRuntime = m_pCrrActionRuntime->GetEventRuntime(m_Idx_EventRuntime);
		if (pEventRuntime->GetStartTime() > m_fElapsedTime)
			break;

		if (pEventRuntime->OnStart(m_fElapsedTime))
			return true;

		if (pEventRuntime->IsDuration())
		{
			if (pEventRuntime->OnTick(m_fElapsedTime, m_fElapsedTime - pEventRuntime->GetStartTime()))
			{
				pEventRuntime->OnEnd(false);
				return true;
			}

			m_llist_EventRuntimesForUpdate.AddTail(pEventRuntime->GetNode());
		}

		++m_Idx_EventRuntime;
	}

	return false;
}

void UActionController::OnEndAction()
{
	// 진행중인 모든 EventRuntime 종료
	auto pNode_Event = m_llist_EventRuntimesForUpdate.GetHead();
	while (pNode_Event)
	{
		auto pCrrNode_Event = pNode_Event;
		pNode_Event = pNode_Event->GetNextNode();

		pCrrNode_Event->GetValue()->OnEnd(false);
		m_llist_EventRuntimesForUpdate.RemoveNode(pCrrNode_Event, false);
	}
	
	m_pCrrActionRuntime = nullptr;
	m_fElapsedTime = 0.f;
	m_Idx_EventRuntime = 0;

	// 켜져있던 모든 루프형 FX, SFX 종료
	if (m_set_LoopingFX.Num() > 0)
	{
		auto fxes = m_set_LoopingFX.Array();
		for (auto fx : fxes)
			fx->Stop();

		m_set_LoopingFX.Empty();
	}

	if (m_set_LoopingSFX.Num() > 0)
	{
		auto sfxes = m_set_LoopingSFX.Array();

		for (auto sfx : m_set_LoopingSFX)
			sfx->ReturnToManager();

		m_set_LoopingSFX.Empty();
	}
}

UActionRuntime* UActionController::CheckNextAction(ETriggerEventTypes eventType)
{
	if (m_pCrrActionRuntime == nullptr)
		return nullptr;
	
	for (auto& triggerRuntime : m_pCrrActionRuntime->GetTriggerRuntimes(eventType))
	{
		// Check Time
		if (!triggerRuntime->CheckTime(m_fElapsedTime, m_pCrrActionRuntime->GetData()->fLength))
			continue;;
		
		auto& triggerData = triggerRuntime->GetData();
		auto& crrTriggerType = m_pOwner->GetBrainCtrl()->GetAxisTriggerType();

		// Check & Play Trigger
		switch(triggerData.AxisType)
		{
			case ETriggerAxisTypes::NoUse:
				return triggerRuntime->GetNextActionRuntime();
			case ETriggerAxisTypes::Any:
				{
					if (crrTriggerType == ETriggerAxisTypes::None)
						continue;
				}
				return triggerRuntime->GetNextActionRuntime();
			default:
				{
					if (crrTriggerType != triggerData.AxisType)
						continue;
				}
				return triggerRuntime->GetNextActionRuntime();
		}
	}

	return nullptr;
}

void UActionController::AddActionData(UActionData* actionData)
{
	// Action 등록 체크
	if (!m_map_ActionRuntime.Contains(actionData))
	{
		auto newActionRuntime = NewObject<UActionRuntime>(this);
		newActionRuntime->Init(actionData, m_pOwner);
		
		m_map_ActionRuntime.Add(actionData, newActionRuntime);
		m_map_ActionRuntimeByName.Add(actionData->GetName(), newActionRuntime);
	}
}

void UActionController::InitActionDatas(const TArray<UActionData*>& actionDatas)
{
	for (auto actionData : actionDatas)
		AddActionData(actionData);

	for (auto pair : m_map_ActionRuntime)
	{
		auto pActionRuntime = pair.Value;
		pActionRuntime->InitTriggers(m_map_ActionRuntimeByName);
	}
}

// 등록되지 않은 액션 절대 사용 금지
void UActionController::PlayAction(UActionRuntime* actionRuntime)
{
	if (!actionRuntime)
		return;

	LOG_WARNING(TEXT("PlayAction :: %s // %s"), *actionRuntime->GetData()->GetName(), *GetOwner()->GetName());

	// 기존 액션 종료
	if (m_pCrrActionRuntime)
		OnEndAction();

	// AI 공격 콤보가 진행 중일떄,
	// Idle이나 HitReaction이 온다면 리셋한다.
	const auto& pBrainCtrl = m_pOwner->GetBrainCtrl();
	if (pBrainCtrl->IsAIInCombo() && actionRuntime->IsResetAI())
		pBrainCtrl->ResetAI();

	// 신규 액션 등록
	m_pCrrActionRuntime = actionRuntime;

	// 변수 초기화
	m_fElapsedTime = 0.f;
	m_Idx_EventRuntime = 0;

	// Start 액션
	CheckStartEvent();
}

void UActionController::PlayAction(const FString& actionName)
{
	const auto pActionRuntime = m_map_ActionRuntimeByName[*actionName];
	PlayAction(pActionRuntime);
}

void UActionController::PlayAction(UActionData* actionData)
{
	if (!actionData)
		return;

	PlayAction(m_map_ActionRuntime[actionData]);
}

void UActionController::StopAction()
{
	if (m_pCrrActionRuntime)
		OnEndAction();
}

bool UActionController::IsPlaying() const
{
	return m_pCrrActionRuntime != nullptr;
}

bool UActionController::IsPlaying(const FString& actionDataName) const
{
	return m_pCrrActionRuntime && m_pCrrActionRuntime->GetData()->GetName().Compare(actionDataName) == 0;
}

const UActionRuntime* UActionController::GetCrrActionRuntime() const
{
	return m_pCrrActionRuntime;
}

const float& UActionController::GetActionElapsedTime() const
{
	return m_fElapsedTime;
}

void UActionController::AddLoopingFX(AFXActor* pFxActor)
{
	if (pFxActor)
		m_set_LoopingFX.Add(pFxActor);
}

void UActionController::RemoveLoopingFX(AFXActor* pFxActor)
{
	if (pFxActor)
		m_set_LoopingFX.Remove(pFxActor);
}

void UActionController::AddLoopingSFX(ASFXActor* pSfxActor)
{
	if (pSfxActor)
		m_set_LoopingSFX.Add(pSfxActor);
}

void UActionController::RemoveLoopingSFX(ASFXActor* pSfxActor)
{
	if (pSfxActor)
		m_set_LoopingSFX.Remove(pSfxActor);
}