// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFXActor.h"

ATextFXActor::ATextFXActor()
{
	m_pTextComp = nullptr;
}

void ATextFXActor::SetText(const FString& text)
{
	m_pTextComp->SetText(FText::FromString(text));
}
