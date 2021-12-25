// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInst.h"
#include "../GameCharacter.h"
#include "Framework/Contents/GameCharacter/BrainControl/BrainController.h"

void UAnimInst::Init(AGameCharacter* pOwner)
{
	m_pOwner = pOwner;
}

void UAnimInst::OnTick(float deltaTime)
{
	if (m_BlendOption == EAnimBlendOption::None)
		return;

	// 높이를 제거한 Velocity
	auto vMoveDir= FVector::VectorPlaneProject(
		m_pOwner->GetCharacterMovement()->Velocity,
		m_pOwner->GetActorUpVector()
		);

	// 이동 속도가 없다면
	auto fMoveSize = vMoveDir.SizeSquared();
	if (fMoveSize <= 0.f)
	{
		m_fMoveSize = 0.f;
		m_fAngleBetweenForwardAndMove = 0.f;
	}
	// 이동 속도가 있다면
	else
	{
		// Move Factor 세팅
		fMoveSize = FMath::Sqrt(fMoveSize);
		auto& fMaxMoveSize = m_pOwner->GetCharacterMovement()->MaxWalkSpeed;
		if (fMaxMoveSize > 0.f)
			m_fMoveSize = fMoveSize / fMaxMoveSize;
		else
			m_fMoveSize = 0.f;

		// Angle 세팅
		auto vDirMove = vMoveDir * (1.f / fMoveSize);
		auto vDirOwner = m_pOwner->GetActorForwardVector();

		m_fAngleBetweenForwardAndMove = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(vDirMove, vDirOwner)));

		// Angle 세팅 :: 왼쪽 오른쪽 판단
		if (m_fAngleBetweenForwardAndMove > 0.f)
		{
			auto vRight = FVector::CrossProduct(vDirMove, vDirOwner);

			// Todo :: 중력 방향이 바뀌는 게임이면 좀더 복잡한 공식 요구
			if (vRight.Z >= 0.f)
				m_fAngleBetweenForwardAndMove *= -1.f;
		}
	}
}

void UAnimInst::Play(UAnimMontage* animMontage, const float& fadeTime, const float& playRate, const bool& bResetIfPlaying, const EAnimBlendOption& blendOption)
{
	if(bResetIfPlaying || !Montage_IsPlaying(animMontage))
	{
		m_BlendTime = fadeTime;
		m_BlendOption = blendOption;
		animMontage->BlendIn.SetBlendTime(m_MontageIsPlaying ? fadeTime : 0.f);
		animMontage->BlendOut.SetBlendTime(fadeTime);

		Montage_Play(animMontage, playRate);
		m_MontageIsPlaying = true;
	}
	else
	{
		Montage_SetPlayRate(animMontage, playRate);
		m_MontageIsPlaying = true;
	}
}

void UAnimInst::Stop(const float& fOutTime, const EAnimBlendOption& blendOption)
{
	m_MontageIsPlaying = false;
	m_BlendTime = fOutTime;
	m_BlendOption = blendOption;
	Montage_Stop(m_BlendTime);
}