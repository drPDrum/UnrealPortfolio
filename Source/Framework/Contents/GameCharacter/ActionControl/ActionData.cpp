// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionData.h"

UActionData::UActionData()
{
}

UActionData::~UActionData()
{
}

void UActionData::AddEvent(FActionEventDataBase* pEventData)
{
	EventDatas.Add(pEventData);

	auto actionEventType = pEventData->GetActionEventType();
	switch(actionEventType)
	{
	case ActionEventTypes::FindTarget			: EventDatas_FindTarget				.Add(*(FActionEventData_FindTarget*		)pEventData);	break;
		
	case ActionEventTypes::MoveByInputEntered	: EventDatas_MoveByInputEntered		.Add(*(FActionEventData_MoveInputE*		)pEventData);	break;
	case ActionEventTypes::MoveByInputUpdated	: EventDatas_MoveByInputUpdated		.Add(*(FActionEventData_MoveInputU*		)pEventData);	break;
	case ActionEventTypes::MoveToTargetEntered	: EventDatas_MoveToTargetEntered	.Add(*(FActionEventData_MoveTargetE*	)pEventData);	break;
	case ActionEventTypes::MoveToTargetUpdated	: EventDatas_MoveToTargetUpdated	.Add(*(FActionEventData_MoveTargetU*	)pEventData);	break;
	case ActionEventTypes::MoveNavEntered		: EventDatas_MoveNavEntered			.Add(*(FActionEventData_MoveNavE*		)pEventData);	break;
	case ActionEventTypes::MoveNavUpdated		: EventDatas_MoveNavUpdated			.Add(*(FActionEventData_MoveNavU*		)pEventData);	break;
		
	case ActionEventTypes::Rotate	 			: EventDatas_Rotate					.Add(*(FActionEventData_Rotate*			)pEventData);	break;
	case ActionEventTypes::RotateByInputEntered	: EventDatas_RotateByInputEntered	.Add(*(FActionEventData_RotateInputE*	)pEventData);	break;
	case ActionEventTypes::RotateByInputUpdated	: EventDatas_RotateByInputUpdated	.Add(*(FActionEventData_RotateInputU*	)pEventData);	break;
	case ActionEventTypes::RotateToTargetEntered: EventDatas_RotateToTargetEntered	.Add(*(FActionEventData_RotateTargetE*	)pEventData);	break;
	case ActionEventTypes::RotateToTargetUpdated: EventDatas_RotateToTargetUpdated	.Add(*(FActionEventData_RotateTargetU*	)pEventData);	break;
	case ActionEventTypes::RotateToVelocity		: EventDatas_RotateToVelocity		.Add(*(FActionEventData_RotateVel*		)pEventData);	break;

	case ActionEventTypes::Animation 			: EventDatas_Anim					.Add(*(FActionEventData_Animation*		)pEventData);	break;
	case ActionEventTypes::PlaySFX	 			: EventDatas_PlaySFX				.Add(*(FActionEventData_PlaySFX*		)pEventData);	break;
	case ActionEventTypes::PlayFX	 			: EventDatas_PlayFX					.Add(*(FActionEventData_PlayFX*			)pEventData);	break;
	case ActionEventTypes::HitShape	 			: EventDatas_HitShape				.Add(*(FActionEventData_HitShape*		)pEventData);	break;
	default:																																break;
	}
}

void UActionData::PrevSave()
{
	EventDatas.Empty();
}

void UActionData::AfterLoad()
{
	EventDatas.Empty();

	int nDataCount = 0;

	// Find
	nDataCount = EventDatas_FindTarget.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_FindTarget[i]);

	// Move
	nDataCount = EventDatas_MoveByInputEntered.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_MoveByInputEntered[i]);

	nDataCount = EventDatas_MoveByInputUpdated.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_MoveByInputUpdated[i]);
	
	nDataCount = EventDatas_MoveToTargetEntered.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_MoveToTargetEntered[i]);
	
	nDataCount = EventDatas_MoveToTargetUpdated.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_MoveToTargetUpdated[i]);
	
	nDataCount = EventDatas_MoveNavEntered.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_MoveNavEntered[i]);
	
	nDataCount = EventDatas_MoveNavUpdated.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_MoveNavUpdated[i]);

	// Rotate
	nDataCount = EventDatas_Rotate.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_Rotate[i]);

	nDataCount = EventDatas_RotateByInputEntered.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_RotateByInputEntered[i]);

	nDataCount = EventDatas_RotateByInputUpdated.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_RotateByInputUpdated[i]);
	
	nDataCount = EventDatas_RotateToTargetEntered.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_RotateToTargetEntered[i]);
	
	nDataCount = EventDatas_RotateToTargetUpdated.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_RotateToTargetUpdated[i]);
	
	nDataCount = EventDatas_RotateToVelocity.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_RotateToVelocity[i]);

	// Anim
	nDataCount = EventDatas_Anim.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_Anim[i]);

	// SFX
	nDataCount = EventDatas_PlaySFX.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_PlaySFX[i]);

	// FX
	nDataCount = EventDatas_PlayFX.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_PlayFX[i]);

	// HIT
	nDataCount = EventDatas_HitShape.Num();
	for (int i = 0 ; i < nDataCount; ++i)
		EventDatas.Add(&EventDatas_HitShape[i]);

	Algo::Sort(EventDatas, [](const FActionEventDataBase* x, const FActionEventDataBase* y)
	{
		if (x->fStartTime != y->fStartTime)
			return x->fStartTime < y->fStartTime;
		
		return GetEventOrder(x->GetActionEventType()) <  GetEventOrder(y->GetActionEventType());
	});

	Algo::Sort(TriggerDatas, [](const FActionTriggerData& x, const FActionTriggerData& y)
	{
		return x.AxisType < y.AxisType;
	});
}

int UActionData::GetEventOrder(ActionEventTypes actionEventType)
{
	switch(actionEventType)
	{
	case ActionEventTypes::FindTarget			: return 100;

	case ActionEventTypes::MoveByInputEntered	: return 200;
	case ActionEventTypes::MoveByInputUpdated	: return 200;
	case ActionEventTypes::MoveToTargetEntered	: return 200;
	case ActionEventTypes::MoveToTargetUpdated	: return 200;
	case ActionEventTypes::MoveNavEntered		: return 200;
	case ActionEventTypes::MoveNavUpdated		: return 200;
		
	case ActionEventTypes::Rotate	 			: return 300;
	case ActionEventTypes::RotateByInputEntered	: return 300;
	case ActionEventTypes::RotateByInputUpdated	: return 300;
	case ActionEventTypes::RotateToTargetEntered: return 300;
	case ActionEventTypes::RotateToTargetUpdated: return 300;
	case ActionEventTypes::RotateToVelocity		: return 300;

	case ActionEventTypes::HitShape	 			: return 400;
	case ActionEventTypes::Animation 			: return 500;
	case ActionEventTypes::PlaySFX	 			: return 600;
	case ActionEventTypes::PlayFX	 			: return 700;
	default							 			: return 10000;
	}
}