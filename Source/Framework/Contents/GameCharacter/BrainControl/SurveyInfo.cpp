#include "SurveyInfo.h"
#include "../GameCharacter.h"
#include "Engine.h"
#include "../StatControl/StatController.h"

void SurveyGameCharacterInfo::RefreshMinInfo(const float& fNewValue, AGameCharacter* const pNewTarget, FBodyInstance* const pNewBody)
{
	if (fValue < fNewValue)
		return;

	fValue = fNewValue;
	pBody = pNewBody;
	pTarget = pNewTarget;
}

void SurveyGameCharacterInfo::RefreshMaxInfo(const float& fNewValue, AGameCharacter* const pNewTarget, FBodyInstance* const pNewBody)
{
	if (fValue > fNewValue)
		return;

	fValue = fNewValue;
	pBody = pNewBody;
	pTarget = pNewTarget;
}

void SurveyGameCharacterInfo::Clear(float fClearValue)
{
	fValue = fClearValue; 
	pBody = nullptr;
	pTarget = nullptr;
}

USurveyInfo::USurveyInfo()
{
	Clear();
}

USurveyInfo::~USurveyInfo()
{
	Clear();
}

void USurveyInfo::AddInfo(AGameCharacter* const pOwner, const FHitResult& hitResult)
{
	auto pTarget = Cast<AGameCharacter>(hitResult.GetActor());
	if (!pTarget)
		return;
	
	auto pTeamInfo_Owner = pOwner->GetTeamInfo();
	auto pTeamInfo_Target = pTarget->GetTeamInfo();

	if (!(pTeamInfo_Owner && pTeamInfo_Target))
		return;

	auto pTargetBody = HitResultToBodyInst(hitResult);
	
	const auto& idx_Team = pTarget->GetTeamInfo()->GetIdx();
	auto fHPRate = pTarget->GetStatCtrl()->GetCrrHPRate();
	auto fDist = (hitResult.ImpactPoint - pOwner->GetActorLocation()).SizeSquared();

	if (pOwner->GetTeamInfo()->IsFriendly(idx_Team))
	{
		// 살았다면
		if (pTarget->GetStatCtrl()->GetCrrHP() > 0)
		{
			++nCount_Friendly_Alive;

			// Fiendly HPInfo
			Friendly_HPRateNearest.RefreshMinInfo(fHPRate, pTarget, pTargetBody);
			Friendly_HPRateHighest.RefreshMaxInfo(fHPRate, pTarget, pTargetBody);

			// Friendly Distance
			Friendly_Nearest.RefreshMinInfo(fDist, pTarget, pTargetBody);
			Friendly_Farthest.RefreshMaxInfo(fDist, pTarget, pTargetBody);
		}
		// 죽었다면
		else
		{
			++nCount_Friendly_Die;
		}
	}
	else if(pOwner->GetTeamInfo()->IsEnemy(idx_Team))
	{
		// 살았다면
		if (pTarget->GetStatCtrl()->GetCrrHP() <= 0)
		{
			++nCount_Enemy_Alive;

			// Enemy HPInfo
			Enemy_HPRateNearest.RefreshMinInfo(fHPRate, pTarget, pTargetBody);
			Enemy_HPRateHighest.RefreshMaxInfo(fHPRate, pTarget, pTargetBody);

			// Enemy Distance
			Enemy_Nearest.RefreshMinInfo(fDist, pTarget, pTargetBody);
			Enemy_Farthest.RefreshMaxInfo(fDist, pTarget, pTargetBody);
		}
		// 죽었다면
		else
		{
			++nCount_Enemy_Die;
		}
	}
}

const SurveyGameCharacterInfo& USurveyInfo::GetSurveyTargetInfo(const ESurveyTargetTypes& targetType) const
{
	switch (targetType)
	{
	case ESurveyTargetTypes::Friendlay_Nearest:
		return Friendly_Nearest;
	case ESurveyTargetTypes::Friendlay_Farthest:
		return Friendly_Farthest;
	case ESurveyTargetTypes::Friendly_HP_Highest:
		return Friendly_HPRateHighest;
	case ESurveyTargetTypes::Friendly_HP_Lowest:	
		return Friendly_HPRateNearest;
	case ESurveyTargetTypes::Enemy_Nearest:			
		return Enemy_Nearest;
	case ESurveyTargetTypes::Enemy_Farthest:		
		return Enemy_Farthest;
	case ESurveyTargetTypes::Enemy_HP_Highest:		
		return Enemy_HPRateHighest;
	case ESurveyTargetTypes::Enemy_HP_Lowest:		
		return Enemy_HPRateNearest;
	}
	
	return Enemy_Nearest;
}

void USurveyInfo::Clear()
{
	nCount_Friendly_Alive	= 0;
	nCount_Friendly_Die		= 0;
	nCount_Enemy_Alive		= 0;
	nCount_Enemy_Die		= 0;

	Friendly_Nearest		.Clear(std::numeric_limits<float>::max());
	Friendly_Farthest		.Clear(0.f);
	Friendly_HPRateHighest	.Clear(0.f);
	Friendly_HPRateNearest	.Clear(std::numeric_limits<float>::max());
	Enemy_Nearest			.Clear(std::numeric_limits<float>::max());
	Enemy_Farthest			.Clear(0.f);
	Enemy_HPRateHighest		.Clear(0.f);
	Enemy_HPRateNearest		.Clear(std::numeric_limits<float>::max());
}