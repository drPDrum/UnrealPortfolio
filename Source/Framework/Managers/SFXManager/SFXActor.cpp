// Fill out your copyright notice in the Description page of Project Settings.


#include "SFXActor.h"

#include "SFXManager.h"
#include "../Managers.h"

// Sets default values
ASFXActor::ASFXActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SetCanBeDamaged(false);
	
	m_Node_Pool = new TDoubleLinkedListEx<ASFXActor*>::TDoubleLinkedListExNode(this);
	m_AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	SetRootComponent(m_AudioComp);
}

ASFXActor::~ASFXActor()
{
	m_Node_Pool->RemoveSelf();
	delete m_Node_Pool;
}


// Called when the game starts or when spawned
void ASFXActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASFXActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_AudioComp->GetPlayState() == EAudioComponentPlayState::Stopped)
		ReturnToManager();
}

bool ASFXActor::PlaySFX(USoundBase* sound, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation)
{
	if (sound)
	{
		if (parent)
		{
			// Attach
			FAttachmentTransformRules attachmentRules =
				FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

			AttachToActor(parent, attachmentRules, socketName);
			SetActorRelativeLocation(location);
			SetActorRelativeRotation(rotation);

			if (!bFollowParent)
			{
				// Detach
				FDetachmentTransformRules detachmentRules =
					FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	
				DetachFromActor(detachmentRules);
			}
		}
		else
		{
			FDetachmentTransformRules detachmentRules =
				FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	
			DetachFromActor(detachmentRules);
			SetActorLocation(location);
			SetActorRotation(rotation);
		}

		// Enable
		SetActorTickEnabled(true);
		SetActorEnableCollision(true);
		SetActorHiddenInGame(false);
		
		// Sound
		m_AudioComp->SetSound(sound);
		m_AudioComp->Play();
		return true;
	}

	return false;
}

bool ASFXActor::Play(USoundBase* sound, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation)
{
	if (m_OnReturn.IsBound())
	{
		auto callback = m_OnReturn;
		m_OnReturn.Unbind();
		callback.Execute(this);
	}

	return PlaySFX(sound, parent, socketName, bFollowParent, location, rotation);
}


bool ASFXActor::Play(USoundBase* sound, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FOnSFXReturn& onReturn)
{
	if (m_OnReturn.IsBound())
	{
		auto callback = m_OnReturn;
		m_OnReturn = onReturn;
		callback.Execute(this);
	}
	else
	{
		m_OnReturn = onReturn;
	}

	return PlaySFX(sound, parent, socketName, bFollowParent, location, rotation);
}

void ASFXActor::ReturnToManager()
{
	// Sound
	m_AudioComp->Stop();
	m_AudioComp->SetSound(nullptr);

	// Detach
	FDetachmentTransformRules detachmentRules =
		FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	
	DetachFromActor(detachmentRules);

	// Disable
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	// Retrieve
	Managers::SFX->RetrieveSFX(this);

	// Return Callback
	if (m_OnReturn.IsBound())
	{
		auto callback = m_OnReturn;
		m_OnReturn.Unbind();
		callback.Execute(this);
	}
}

bool ASFXActor::IsLoop()
{
	if (m_AudioComp->Sound)
		return m_AudioComp->Sound->IsLooping();

	return false;
}

TDoubleLinkedListEx<ASFXActor*>::TDoubleLinkedListExNode* ASFXActor::GetNode()
{
	return m_Node_Pool;
}

