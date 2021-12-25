// Fill out your copyright notice in the Description page of Project Settings.


#include "EmptyBoxCollider.h"

// Sets default values
AEmptyBoxCollider::AEmptyBoxCollider()
{
	m_pBoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	m_pBoxComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_pBoxComponent->CanCharacterStepUpOn = ECB_No;

	SetRootComponent(m_pBoxComponent);
}