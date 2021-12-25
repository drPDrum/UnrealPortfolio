// Fill out your copyright notice in the Description page of Project Settings.


#include "FXActor.h"

#include "FXManager.h"
#include "../Managers.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "GhostComponent.h"

// Sets default values
AFXActor::AFXActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Node_Pool = new TDoubleLinkedListEx<AFXActor*>::TDoubleLinkedListExNode(this);
	m_pContainer = nullptr;

	m_State =  EFXStateTypes::Stopped;
	m_bIsLoop = false;
	m_nParticleCount = 0;
	m_nRemainParticleCount = 0;
}

AFXActor::~AFXActor()
{
	m_Node_Pool->RemoveSelf();
	delete m_Node_Pool;
}

// Called when the game starts or when spawned
void AFXActor::BeginPlay()
{
	// Ghost의 매쉬 컴포넌트 획득 방지를 위해 Super 전에 호출
	GetComponents<UMeshComponent>(m_Meshes);

	Super::BeginPlay();

	GetComponents<UParticleSystemComponent>(m_Particles);
	GetComponents<UNiagaraComponent>(m_Niagara);
	GetComponents<UGhostComponent>(m_Ghost);

	m_nParticleCount = m_Particles.Num() + m_Niagara.Num() + m_Ghost.Num();

	// 루프형인지 체크
	for (auto pParticle : m_Particles)
	{
		if (pParticle->Template->IsLooping())
			m_bIsLoop = true;

		pParticle->OnSystemFinished.AddUniqueDynamic(this, &AFXActor::OnParticleSystemFinished);
		pParticle->bAutoDestroy = false;
	}

	for (auto pNiagara : m_Niagara)
	{
		if (pNiagara->GetAsset()->IsLooping())
			m_bIsLoop = true;
		
		pNiagara->OnSystemFinished.AddUniqueDynamic(this, &AFXActor::OnNiagaraSystemFinished);
		pNiagara->SetAutoDestroy(false);
	}
	
	for (auto pGhost : m_Ghost)
	{
		if (pGhost->IsLooping())
			m_bIsLoop = true;
		
		pGhost->OnGhostFinished.AddUniqueDynamic(this, &AFXActor::OnGhostFinished);
	}
}

// Called every frame
void AFXActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFXActor::Init(FXContainer* myContainer)
{
	m_pContainer = myContainer;
}

bool AFXActor::PlayFX(AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale)
{
	m_State = EFXStateTypes::Playing;
	if (parent)
	{
		// Attach
		FAttachmentTransformRules attachmentRules =
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);

		AttachToActor(parent, attachmentRules, socketName);
		SetActorRelativeLocation(location);
		SetActorRelativeRotation(rotation);
		SetActorRelativeScale3D(scale);
		
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
		SetActorScale3D(scale);
	}		

	// Enable
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);

	m_nRemainParticleCount = m_nParticleCount;
	
	for (auto pParticle : m_Particles)
		pParticle->Activate(true);

	for (auto pNiagara : m_Niagara)
		pNiagara->Activate(true);
	
	for (auto pGhost : m_Ghost)
		pGhost->Activate(true);

	return true;
}

bool AFXActor::Play(AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale)
{
	if (m_OnReturn.IsBound())
	{
		auto callback = m_OnReturn;
		m_OnReturn.Unbind();
		callback.Execute(this);
	}

	return PlayFX(parent, socketName, bFollowParent, location, rotation, scale);
}


bool AFXActor::Play(AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale, const FOnFXReturn& onReturn)
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

	return PlayFX(parent, socketName, bFollowParent, location, rotation, scale);
}

void AFXActor::Stop()
{
	m_State = EFXStateTypes::WaitForReturn;
	
	for (auto pParticle : m_Particles)
		pParticle->Deactivate();
	
	for (auto pNiagara : m_Niagara)
		pNiagara->Deactivate();
	
	for (auto pGhost : m_Ghost)
		pGhost->Deactivate();

	// Return Callback
	if (m_OnReturn.IsBound())
	{
		auto callback = m_OnReturn;
		m_OnReturn.Unbind();
		callback.Execute(this);
	}

	CheckAllComponentEnd();
}

void AFXActor::ReturnToManager()
{
	m_State = EFXStateTypes::Stopped;
	for (auto pParticle : m_Particles)
		pParticle->DeactivateImmediate();

	for (auto pNiagara : m_Niagara)
		pNiagara->DeactivateImmediate();

	for (auto pGhost : m_Ghost)
		pGhost->DeactivateImmediate();
	
	// Detach
	FDetachmentTransformRules detachmentRules =
		FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	
	DetachFromActor(detachmentRules);

	// Disable
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);

	// Retrieve
	Managers::FX->RetrieveFX(this);

	// Return Callback
	if (m_OnReturn.IsBound())
	{
		auto callback = m_OnReturn;
		m_OnReturn.Unbind();
		callback.Execute(this);
	}
}

bool AFXActor::IsLooping()
{
	return m_bIsLoop;
}

bool AFXActor::HasGhost()
{
	return m_Ghost.Num() > 0;
}

void AFXActor::SetGhost(const TArray<USkeletalMeshComponent*>& skeletalMeshComps, const TArray<UStaticMeshComponent*>& staticMeshComps)
{
	for (auto pGhost : m_Ghost)
		pGhost->SetTarget(skeletalMeshComps, staticMeshComps);
}

void AFXActor::SetGhost(const TArray<USkeletalMeshComponent*>& skeletalMeshComps)
{
	for (auto pGhost : m_Ghost)
		pGhost->SetTarget(skeletalMeshComps);
}

void AFXActor::SetGhost(const TArray<UStaticMeshComponent*>& staticMeshComps)
{
	for (auto pGhost : m_Ghost)
		pGhost->SetTarget(staticMeshComps);
}

TDoubleLinkedListEx<AFXActor*>::TDoubleLinkedListExNode* AFXActor::GetNode() const
{
	return m_Node_Pool;
}

FXContainer* AFXActor::GetContainer()
{
	return m_pContainer;
}

void AFXActor::OnParticleSystemFinished(UParticleSystemComponent* pParticle)
{
	if (m_State == EFXStateTypes::Stopped)
		return;
	
	--m_nRemainParticleCount;
	CheckAllComponentEnd();
}

void AFXActor::OnNiagaraSystemFinished(UNiagaraComponent* pParticle)
{
	if (m_State == EFXStateTypes::Stopped)
		return;
	
	--m_nRemainParticleCount;
	CheckAllComponentEnd();
}

void AFXActor::OnGhostFinished(UGhostComponent* pGhost)
{
	if (m_State == EFXStateTypes::Stopped)
		return;
	
	--m_nRemainParticleCount;
	CheckAllComponentEnd();
}

void AFXActor::CheckAllComponentEnd()
{
	if (m_nRemainParticleCount > 0)
		return;

	ReturnToManager();
}