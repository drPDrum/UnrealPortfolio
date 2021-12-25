// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionEventData_HitShape.h"
#include "ActionEventRuntime_HitShape.h"
#include "../ActionController.h"

FHitShapeInfo::FHitShapeInfo()
{
	m_CastType = ECastTypes::MyPos_Box;
	m_fDist = 0.f;

	m_vOffsetPos = FVector(100.f, 0.f, 0.f);
	m_qOffsetRot = FQuat::Identity;
	m_Size = FVector(200.f, 200.f, 200.f);
}

FHitShapeInfo::~FHitShapeInfo()
{
}

FActionEventData_HitShape::FActionEventData_HitShape()
{
	m_TargetType = EGameCharacterFindTargetTypes::Enemy;
	
	m_bIsFollow = false;
}

FActionEventData_HitShape::~FActionEventData_HitShape()
{
}

CreateRuntimeResult FActionEventData_HitShape::CreateRuntime(AGameCharacter* owner)
{
	auto pNewRuntime = NewObject<UActionEventRuntime_HitShape>(owner->GetActionCtrl());
	pNewRuntime->Init(*this, owner);
	
	CreateRuntimeResult result;

	result.pUObject = pNewRuntime;
	result.pRuntime = pNewRuntime;

	return result;
}

ActionEventTypes FActionEventData_HitShape::GetActionEventType() const
{
	return ActionEventTypes::HitShape;
}
