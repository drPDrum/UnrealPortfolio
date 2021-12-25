// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_WaitActionType.h"

#include "AIController.h"
#include "Framework/Contents/GameCharacter/GameCharacter.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionController.h"
#include "Framework/Contents/GameCharacter/ActionControl/ActionRuntime.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"
#include "RuntimeUtils/GenericEx/Common.h"

UBTTask_WaitActionType::UBTTask_WaitActionType()
{
	bNotifyTick = true;
}

UBTTask_WaitActionType::~UBTTask_WaitActionType()
{
}

EBTNodeResult::Type UBTTask_WaitActionType::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// owner 체크
	auto pOwner = Cast<AGameCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pOwner)
		return EBTNodeResult::Succeeded;

	// 콤보 종료를 명시
	pOwner->GetBrainCtrl()->SetAIInCombo(false);

	auto crrActionRuntime = pOwner->GetActionCtrl()->GetCrrActionRuntime();
	if (!crrActionRuntime)
		return EBTNodeResult::Succeeded;
	
	if (m_WaitActionType != crrActionRuntime->GetData()->actionType)
		return EBTNodeResult::InProgress;

	return EBTNodeResult::Failed;
}

void UBTTask_WaitActionType::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	auto pOwner = Cast<AGameCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!pOwner)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	const auto& pActonCtrl =  pOwner->GetActionCtrl();

	auto crrActionRuntime = pActonCtrl->GetCrrActionRuntime();
	if (!crrActionRuntime)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	
	if (m_WaitActionType != crrActionRuntime->GetData()->actionType)
		return;

	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}

EBTNodeResult::Type UBTTask_WaitActionType::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto result = Super::AbortTask(OwnerComp, NodeMemory);
	
	return result;
}

FString UBTTask_WaitActionType::GetStaticDescription() const
{
	return CachedDescription;
}

#if WITH_EDITOR


void UBTTask_WaitActionType::BuildDescription()
{
	CachedDescription.Empty();
	CachedDescription.Append(FString::Printf(TEXT("ActionType : %s"), *UEnumToString(m_WaitActionType)));
}

void UBTTask_WaitActionType::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	BuildDescription();
}

void UBTTask_WaitActionType::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	BuildDescription();
}
#endif