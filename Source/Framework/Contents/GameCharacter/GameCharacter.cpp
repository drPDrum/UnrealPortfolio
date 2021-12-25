// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"

#include "NavigationSystem.h"
#include "ActionControl/ActionController.h"
#include "AnimControl/AnimController.h"
#include "StatControl/StatController.h"

#include "Framework/Managers/Managers.h"
#include "BrainControl/BrainController.h"
#include "Framework/UserInfo/UserData/CUserCharData.h"

// Sets default values
AGameCharacter::AGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pUserData = nullptr;

	m_ActionCtrl = nullptr;
	m_AnimCtrl = nullptr;
	m_StatCtrl = nullptr;

	AIControllerClass = ABrainController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	m_pNode_State = new TDoubleLinkedListEx<AGameCharacter*>::TDoubleLinkedListExNode(this);

	bUseControllerRotationYaw = false;
	const auto& pMovementComp = GetCharacterMovement();
	if (pMovementComp)
	{
		pMovementComp->RotationRate = FRotator::ZeroRotator;
	}

	m_bActive = true;
	bEnableAutoLODGeneration = false;
}

AGameCharacter::~AGameCharacter()
{
	// if(IsValid(m_BrainCtrl))
	// 	m_BrainCtrl->Destroy();
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	// 시작하자 마자 틱부터 끈다.
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

void AGameCharacter::BeginDestroy()
{
	// 노드 파괴
	m_pNode_State->RemoveSelf();
	delete m_pNode_State;
	
	Super::BeginDestroy();
}

// Called every frame
void AGameCharacter::Tick(float DeltaTime)
{
	// 지역 체크
	// TODO :: 더 깔끔한 방법이 있을 것
	auto pMoveComp = GetCharacterMovement();
	if (pMoveComp->IsMovingOnGround())
	{
		m_Area = EGameCharacterAreaTyeps::Ground;
	}
	else
	{
		m_Area = EGameCharacterAreaTyeps::Air;
	}
	
	m_BrainCtrl->OnOwnerTick(DeltaTime);
	m_ActionCtrl->OnOwnerTick(DeltaTime);
	m_AnimCtrl->OnOwnerTick(DeltaTime);

	Super::Tick(DeltaTime);
}

const CCharacterData* AGameCharacter::GetData() const
{
	if(m_pUserData)
		return m_pUserData->GetData();
	return nullptr;
}

const CStatData* AGameCharacter::GetStatData() const
{
	if(m_pUserData)
		return m_pUserData->GetStatData();
	return nullptr;
}

UActionController* const& AGameCharacter::GetActionCtrl()
{
	return m_ActionCtrl;
}

UAnimController* const& AGameCharacter::GetAnimCtrl()
{
	return m_AnimCtrl;
}

UStatController* const& AGameCharacter::GetStatCtrl()
{
	return m_StatCtrl;
}

ABrainController* const& AGameCharacter::GetBrainCtrl()
{
	return m_BrainCtrl;
}

const EGameCharacterAreaTyeps& AGameCharacter::GetArea()
{
	return m_Area;
}

void AGameCharacter::Hit(AGameCharacter* pAttacker, const FVector& hitPoint, const FQuat& hitNormal, USoundBase* hitSFX, FXContainer* hitFX)
{
	//TODO

	Managers::SFX->PlaySFX(hitSFX, hitPoint, hitNormal);
	Managers::FX->PlayFX(hitFX, hitPoint, hitNormal, pAttacker->GetActorScale());
}

void AGameCharacter::SetNavLocation(const FVector& vLocation)
{
	SetActorLocation(CalNavLocation(vLocation));
}

FVector AGameCharacter::CalNavLocation(const FVector& vLocation) const
{
	FVector result = vLocation;

	auto pNavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (pNavSystem)
	{
		FNavLocation NavLoc;
		if (pNavSystem->ProjectPointToNavigation(vLocation, NavLoc, FVector::ZeroVector, &GetNavAgentPropertiesRef()))
			result = NavLoc.Location;
	}

	result.Z += GetSimpleCollisionHalfHeight();
	return result;
}

void AGameCharacter::SetActive(const bool& bActive)
{
	if(m_bActive == bActive)
		return;
	
	auto bReverse = !bActive;
	
	// Actor
	SetActorTickEnabled(bActive);
	SetActorEnableCollision(bActive);
	SetActorHiddenInGame(bReverse);

	GetMesh()->SetEnableGravity(bActive);
	GetCapsuleComponent()->SetEnableGravity(bActive);

	if (bActive)
		GetMovementComponent()->Activate();
	else
		GetMovementComponent()->Deactivate();

	GetMovementComponent()->Velocity = FVector::ZeroVector;

	// Possess
	if (m_BrainCtrl && !m_BrainCtrl->GetOrderByUserInput())
	{
		if (bActive)
			m_BrainCtrl->Possess(this);
		else
			m_BrainCtrl->UnPossess();
	}
	m_bActive = bActive;
}

bool AGameCharacter::IsActive() const
{
	return m_bActive;
}
