// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimController.h"

UAnimController::UAnimController()
{
	m_pOwner = nullptr;
	m_pAnimInstacne = nullptr;
}

UAnimController::~UAnimController()
{
}


void UAnimController::Init(AGameCharacter* owner)
{
	m_pOwner = owner;
}

AGameCharacter* UAnimController::GetOwner()
{
	return m_pOwner;
}

void UAnimController::SetAnimInst(TSubclassOf<UAnimInst> animInstClass)
{
	auto mesh = m_pOwner->GetMesh();
	if (mesh == nullptr)
		return;

	mesh->SetAnimInstanceClass(animInstClass);
	GetAnimInst()->Init(m_pOwner);

	//기존 AnimInstance는
	//UObject는 GC에서 처리된다
}

UAnimInst* UAnimController::GetAnimInst()
{
	return Cast<UAnimInst>(m_pOwner->GetMesh()->GetAnimInstance());
}

void UAnimController::Play(UAnimMontage* animMontage, const float& fadeTime, const float& playRate, const bool& bResetIfPlaying, const EAnimBlendOption& blendOption)
{
	auto animInst = GetAnimInst();
	if (animInst == nullptr)
		return;

	animInst->Play(animMontage, fadeTime, playRate, bResetIfPlaying, blendOption);
}

void UAnimController::Stop(const float& fOutTime, const EAnimBlendOption& blendOption)
{
	auto animInst = GetAnimInst();
	if (animInst == nullptr)
		return;

	animInst->Stop(fOutTime, blendOption);
}

void UAnimController::OnOwnerTick(float deltaTime)
{
	auto animInst = GetAnimInst();
	if (animInst)
		animInst->OnTick(deltaTime);
}