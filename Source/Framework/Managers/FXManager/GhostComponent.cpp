// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostComponent.h"

#include "Framework/Managers/Managers.h"
#include "Components/PoseableMeshComponent.h"

TMap<uint32, UGhostComponent::MatContainer> UGhostComponent::map_Materials;

// Sets default values for this component's properties
UGhostComponent::UGhostComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_OpacityParamName = TEXT("Opacity");
		
	m_nUseCount_PoseableMesh = 0;
	m_nUseCount_StaticMesh = 0;
}

// Called when the game starts
void UGhostComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 기본 량을 상황에 맞게 조절해주자. 일단 1개씩 세팅한다.
	for (int i = 0 ; i < 1; ++i)
	{
		auto pNewComp = NewObject<UPoseableMeshComponent>(this, *FString::Printf(TEXT("GhostPoseableMeshComponent%d"), i));

		pNewComp->SetupAttachment(this);
		GetOwner()->AddInstanceComponent(pNewComp);
		pNewComp->RegisterComponent();

		pNewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		pNewComp->SetHiddenInGame(true);
		pNewComp->SetVisibility(false);

		m_PoseableMeshComp.Add(pNewComp);
	}
	
	for (int i = 0 ; i < 1; ++i)
	{
		auto pNewComp = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("GhostStaticMeshComponent%d"), i));
		pNewComp->SetupAttachment(this);

		GetOwner()->AddInstanceComponent(pNewComp);
		pNewComp->RegisterComponent();
		pNewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		pNewComp->SetHiddenInGame(true);
		pNewComp->SetVisibility(false);

		m_StaticMeshComp.Add(pNewComp);
	}

	// 머티리얼 컨테이너 생성 and 찾기
	if (!map_Materials.Contains(m_MatFX->GetUniqueID()))
		map_Materials.Add(m_MatFX->GetUniqueID(), MatContainer());

	m_pMatFxContainer = &map_Materials[m_MatFX->GetUniqueID()];
	++(m_pMatFxContainer->RefCount);
}

void UGhostComponent::BeginDestroy()
{
	Super::BeginDestroy();

	ClearAllMeshComp();

	// 컨테이너 해제
	if (m_pMatFxContainer && --(m_pMatFxContainer->RefCount) == 0)
	{
		if (Managers::GameInst)
			Managers::GameInst->RemoveMat(m_pMatFxContainer->Materials);
		m_pMatFxContainer = nullptr;
		map_Materials.Remove(m_MatFX->GetUniqueID());
	}
}


// Called every frame
void UGhostComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch(m_State)
	{
		case EGhostState::Playing:
			if (m_fRemainTime >= 0.f)
			{
				m_fRemainTime -= DeltaTime;
				if (m_fRemainTime <= 0.f)
					Deactivate();
			}
			break;
		case EGhostState::Release:
			if (m_fRemainTime >= 0.f)
			{
				m_fRemainTime -= DeltaTime;
				if (m_fRemainTime <= 0.f)
				{
					DeactivateImmediate();
				}
				else
				{
					float fOpacity = m_fRemainTime / m_fFadeTime;
					for (auto& pMat : m_UseMat_Poseable)
						pMat->SetScalarParameterValue(*m_OpacityParamName, fOpacity);
					for (auto& pMat : m_UseMat_Static)
						pMat->SetScalarParameterValue(*m_OpacityParamName, fOpacity);
				}
			}
			break;
		case EGhostState::Stopped:
			break;
	}
}

void UGhostComponent::SetTarget(const TArray<USkeletalMeshComponent*>& skeletalMeshComps, const TArray<UStaticMeshComponent*>& meshComps)
{
	SetTarget(skeletalMeshComps);
	SetTarget(meshComps);
}

void UGhostComponent::SetTarget(const TArray<USkeletalMeshComponent*>& skeletalMeshComps)
{
	// 기존에 쓰던거 정리
	ClearPoseableMeshComp();

	if (skeletalMeshComps.Num() == 0)
		return;

	m_nUseCount_PoseableMesh = skeletalMeshComps.Num();

	// 더 필요하면 추가
	if (m_PoseableMeshComp.Num() < skeletalMeshComps.Num())
	{
		auto pNewComp = NewObject<UPoseableMeshComponent>(this, *FString::Printf(TEXT("GhostPoseableMeshComponent%d"), m_PoseableMeshComp.Num()));
		pNewComp->SetupAttachment(pNewComp);
		GetOwner()->AddInstanceComponent(pNewComp);
		pNewComp->RegisterComponent();
		
		m_PoseableMeshComp.Add(pNewComp);
	}

	// 세팅
	for (int i = 0 ; i < m_nUseCount_PoseableMesh; ++i)
	{
		auto& pDstMeshComp = m_PoseableMeshComp[i];
		auto& pSrcMeshComp = skeletalMeshComps[i];

		// 매쉬 세팅
		pDstMeshComp->SetSkeletalMesh(pSrcMeshComp->SkeletalMesh);

		// 머티리얼 세팅
		auto matRequierdCount = pSrcMeshComp->GetNumMaterials();
		auto matRemainCount = m_pMatFxContainer->Materials.Num();

		// 머티리얼 세팅 :: 컨테이너에 남은것
		int matCount = FMath::Min(matRequierdCount, matRemainCount);
		for (int j = 0 ; j < matCount ; ++j)
		{
			auto& pMat = m_pMatFxContainer->Materials[--matRemainCount];
			m_UseMat_Poseable.Add(pMat);
			pDstMeshComp->SetMaterial(j, pMat);

			pMat->SetScalarParameterValue(*m_OpacityParamName, 1.f);
		}

		// 머티리얼 세팅 :: 컨테이너 갱신
		m_pMatFxContainer->Materials.SetNum(matRemainCount);

		// 머티리얼 세팅 :: 신규 생성
		for (int j = matCount; j < matRequierdCount; ++j)
		{
			auto pMat = pDstMeshComp->CreateDynamicMaterialInstance(j, m_MatFX);
			m_UseMat_Poseable.Add(pMat);
			Managers::GameInst->AddMat(pMat);

			pMat->SetScalarParameterValue(*m_OpacityParamName, 1.f);
		}

		// 켜준다.
		pDstMeshComp->SetWorldLocation(pSrcMeshComp->GetComponentLocation());
		pDstMeshComp->SetWorldRotation(pSrcMeshComp->GetComponentRotation());
		pDstMeshComp->SetWorldScale3D(pSrcMeshComp->GetComponentScale());
		pDstMeshComp->CopyPoseFromSkeletalComponent(pSrcMeshComp);
		pDstMeshComp->SetVisibility(true);
		pDstMeshComp->SetHiddenInGame(false);
	}
}

void UGhostComponent::SetTarget(const TArray<UStaticMeshComponent*>& meshComps)
{
	// 기존에 쓰던거 정리
	ClearStaticMeshComp();

	if (meshComps.Num() == 0)
		return;
	
	m_nUseCount_StaticMesh = meshComps.Num();
	
	// 더 필요하면 추가
	if (m_StaticMeshComp.Num() < meshComps.Num())
	{
		auto pNewComp = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("GhostStaticMeshComponent%d"), m_StaticMeshComp.Num()));

		pNewComp->SetupAttachment(pNewComp);
		GetOwner()->AddInstanceComponent(pNewComp);
		pNewComp->RegisterComponent();
		m_StaticMeshComp.Add(pNewComp);
	}
	
	// 세팅
	for (int i = 0 ; i < m_nUseCount_StaticMesh; ++i)
	{
		auto& pDstMeshComp = m_StaticMeshComp[i];
		auto& pSrcMeshComp = meshComps[i];

		// 매쉬 세팅
		pDstMeshComp->SetStaticMesh(pSrcMeshComp->GetStaticMesh());

		// 머티리얼 세팅
		auto matRequierdCount = pSrcMeshComp->GetNumMaterials();
		auto matRemainCount = m_pMatFxContainer->Materials.Num();

		// 머티리얼 세팅 :: 컨테이너에 남은것
		int matCount = FMath::Min(matRequierdCount, matRemainCount);
		for (int j = 0 ; j < matCount ; ++j)
		{
			auto& pMat = m_pMatFxContainer->Materials[--matRemainCount];
			m_UseMat_Static.Add(pMat);
			pDstMeshComp->SetMaterial(j, pMat);

			pMat->SetScalarParameterValue(*m_OpacityParamName, 1.f);
		}

		// 머티리얼 세팅 :: 컨테이너 갱신
		m_pMatFxContainer->Materials.SetNum(matRemainCount);

		// 머티리얼 세팅 :: 신규 생성
		for (int j = matCount; j < matRequierdCount; ++j)
		{
			auto pMat = pDstMeshComp->CreateDynamicMaterialInstance(j, m_MatFX);
			m_UseMat_Static.Add(pMat);
			Managers::GameInst->AddMat(pMat);

			pMat->SetScalarParameterValue(*m_OpacityParamName, 1.f);
		}

		// 켜준다.
		pDstMeshComp->SetWorldLocation(pSrcMeshComp->GetComponentLocation());
		pDstMeshComp->SetWorldRotation(pSrcMeshComp->GetComponentRotation());
		pDstMeshComp->SetWorldScale3D(pSrcMeshComp->GetComponentScale());
		pDstMeshComp->SetVisibility(true);
		pDstMeshComp->SetHiddenInGame(false);
	}
}

void UGhostComponent::ClearAllMeshComp()
{
	ClearPoseableMeshComp();
	ClearStaticMeshComp();
}

void UGhostComponent::ClearPoseableMeshComp()
{
	m_UseMat_Poseable.Empty(m_UseMat_Poseable.Max());

	for (int i = 0 ; i < m_nUseCount_PoseableMesh; ++i)
	{
		auto& pMeshComp = m_PoseableMeshComp[i];

		auto mats = pMeshComp->GetMaterials();
		for (auto& pMat : mats)
			m_pMatFxContainer->Materials.Add(Cast<UMaterialInstanceDynamic>(pMat));
		
		pMeshComp->SetSkeletalMesh(nullptr);
		pMeshComp->SetVisibility(false);
		pMeshComp->SetHiddenInGame(true);
	}

	m_nUseCount_PoseableMesh = 0;
}

void UGhostComponent::ClearStaticMeshComp()
{
	m_UseMat_Static.Empty(m_UseMat_Static.Max());

	// 기존에 쓰던거 정리
	for (int i = 0 ; i < m_nUseCount_StaticMesh; ++i)
	{
		auto& pMeshComp = m_StaticMeshComp[i];

		auto mats = pMeshComp->GetMaterials();
		for (auto& pMat : mats)
			m_pMatFxContainer->Materials.Add(Cast<UMaterialInstanceDynamic>(pMat));
		
		pMeshComp->SetStaticMesh(nullptr);
		pMeshComp->SetVisibility(false);
		pMeshComp->SetHiddenInGame(true);
	}

	m_nUseCount_StaticMesh = 0;
}

void UGhostComponent::Activate(bool bReset)
{
	Super::Activate(bReset);

	m_State = EGhostState::Playing;
	m_fRemainTime = m_fLiveTime;

	SetVisibility(true);
	SetHiddenInGame(false);
}

void UGhostComponent::Deactivate()
{
	if (m_State != EGhostState::Playing)
		return;
	
	m_State = EGhostState::Release;
	m_fRemainTime = m_fFadeTime;
	
	OnComponentDeactivated.Broadcast(this);
}

void UGhostComponent::DeactivateImmediate()
{
	if (m_State == EGhostState::Stopped)
		return;

	Super::Deactivate();

	m_State = EGhostState::Stopped;

	ClearAllMeshComp();

	SetVisibility(false);
	SetHiddenInGame(true);
	
	OnGhostFinished.Broadcast(this);
}

bool UGhostComponent::IsLooping() const
{
	return m_fLiveTime < 0.f;
}