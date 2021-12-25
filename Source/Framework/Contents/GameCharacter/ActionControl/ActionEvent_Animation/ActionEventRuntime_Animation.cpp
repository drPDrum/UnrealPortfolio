// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionEventRuntime_Animation.h"
#include "../../AnimControl/AnimController.h"
// #include "../../../../GameContents/GameDatas/GameDataStructs.h"
#include "Framework/Managers/Managers.h"

UActionEventRuntime_Animation::UActionEventRuntime_Animation()
{
	m_AnimMontage = nullptr;
	
}

UActionEventRuntime_Animation::~UActionEventRuntime_Animation()
{
	m_AnimMontage = nullptr;
}

void UActionEventRuntime_Animation::Init(FActionEventData_Animation& Data, AGameCharacter* Owner)
{
	ActionEventRuntimeBase<FActionEventData_Animation>::Init(Data, Owner);

	if (m_pData->m_AnimName.IsEmpty())
		return;
	
	auto ownerData = m_pOwner->GetData();

	// Init Montage
	auto pathFile_AnimMontage = FPaths::Combine(ownerData->RefPath, TEXT("Animations"), m_pData->m_AnimName);
	auto pathRef_AnimMontage = FString::Printf(TEXT("AnimMontage'%s.%s'"), *pathFile_AnimMontage, *(m_pData->m_AnimName));

	m_AnimMontage = Managers::Asset->LoadObj<UAnimMontage>(*pathRef_AnimMontage);
}

bool UActionEventRuntime_Animation::OnStart(float elapsedTime)
{
	if (m_AnimMontage)
	{
		m_pOwner->GetAnimCtrl()->Play(
			m_AnimMontage,
			m_pData->m_fFadeTime,
			m_pData->m_fPlayRate,
			m_pData->m_bResetIfPlaying,
			m_pData->m_BlendOption
			);
	}
	else
	{
		m_pOwner->GetAnimCtrl()->Stop(m_pData->m_fFadeTime, m_pData->m_BlendOption);
	}

	return false;
}