// Fill out your copyright notice in the Description page of Project Settings.


#include "FXManager.h"
#include "FXActor.h"
#include "../Managers.h"


FXContainer::FXContainer()
{
	m_StaticClass = nullptr;
	m_nRefCount = 0;
	m_bIsLoop = false;
}

FXContainer::~FXContainer()
{
	m_StaticClass = nullptr;
	
	// 활성화된 엑터 삭제
	auto node_Active = m_llist_Active.GetHead();
	while (node_Active)
	{
		auto node_Delete = node_Active;
		node_Active = node_Active->GetNextNode();
		node_Delete->RemoveSelf();

		auto pFxActor = node_Delete->GetValue();
		if (pFxActor && IsValid(pFxActor))
			pFxActor->Destroy();
	}

	// 비활성화된 엑터 삭제
	auto node_Inactive = m_llist_Inactive.GetHead();
	while (node_Inactive)
	{
		auto node_Delete = node_Inactive;
		node_Inactive = node_Inactive->GetNextNode();
		node_Delete->RemoveSelf();

		auto pFxActor = node_Delete->GetValue();
		if (pFxActor && IsValid(pFxActor))
			pFxActor->Destroy();
	}
}

const bool& FXContainer::IsLooping() const
{
	return m_bIsLoop;
}

FXManager::FXManager()
{
	
}

FXManager::~FXManager()
{
	for (auto& pair : m_map_FXContainer)
		delete pair.Value;

	m_map_FXContainer.Empty();
}

void FXManager::OnPrevChangeLevel()
{
	// 모든 액터가 파괴 될 것, 회수 및 리스트 초기화
	// 로드 이후에도 컨테이너는 남아있다
	// Generate하면 다시 하나 씩 만들어가면서 컨테이너에 쌓을 것
	for (auto& pair : m_map_FXContainer)
	{
		const auto& pFxContainer = pair.Value;

		// 살아있는 모든 FX 회수
		const auto& node_Fx = pFxContainer->m_llist_Active.GetHead();
		while (node_Fx)
		{
			const auto& pFxActor = node_Fx->GetValue();
			node_Fx->GetNextNode();
			pFxActor->ReturnToManager();
		}
		
		pFxContainer->m_llist_Inactive.Empty(false);
	}
}

AFXActor* FXManager::PlayFX(FString FxName, const FVector& location, const FQuat& rotation, const FVector& scale)
{
	if (m_map_FXContainer.Contains(FxName))
		return PlayFX(m_map_FXContainer[FxName], nullptr, FName(TEXT("None")), false, location, rotation, scale);
	
	return nullptr;
}

AFXActor* FXManager::PlayFX(FString FxName, const FVector& location, const FQuat& rotation, const FVector& scale, const FOnFXReturn& onReturn)
{
	if (m_map_FXContainer.Contains(FxName))
		return PlayFX(m_map_FXContainer[FxName], nullptr, FName(TEXT("None")), false, location, rotation, scale, onReturn);
	
	return nullptr;
}

AFXActor* FXManager::PlayFX(FString FxName, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale)
{
	if (m_map_FXContainer.Contains(FxName))
		return PlayFX(m_map_FXContainer[FxName], parent, socketName, bFollowParent, location, rotation, scale);
	
	return nullptr;
}

AFXActor* FXManager::PlayFX(FString FxName, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale, const FOnFXReturn& onReturn)
{
	if (m_map_FXContainer.Contains(FxName))
		return PlayFX(m_map_FXContainer[FxName], parent, socketName, bFollowParent, location, rotation, scale, onReturn);
	
	return nullptr;
}


AFXActor* FXManager::PlayFX(FXContainer* fxContainer, const FVector& location, const FQuat& rotation, const FVector& scale)
{
	return PlayFX(fxContainer,nullptr, FName(TEXT("None")), false, location, rotation, scale);
}

AFXActor* FXManager::PlayFX(FXContainer* fxContainer, const FVector& location, const FQuat& rotation, const FVector& scale, const FOnFXReturn& onReturn)
{
	return PlayFX(fxContainer,nullptr, FName(TEXT("None")), false, location, rotation, scale, onReturn);
}

AFXActor* FXManager::PlayFX(FXContainer* fxContainer, AActor* parent, const FName& socketName,  const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale)
{
	// Container 유효성 체크
	if (fxContainer)
	{
		AFXActor* pResult = GenerateFX(fxContainer);
		pResult->Play(parent, socketName, bFollowParent, location, rotation, scale);
		return pResult;
	}

	return nullptr;
}

AFXActor* FXManager::PlayFX(FXContainer* fxContainer, AActor* parent, const FName& socketName,  const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale, const FOnFXReturn& onReturn)
{
	// Container 유효성 체크
	if (fxContainer)
	{
		AFXActor* pResult = GenerateFX(fxContainer);
		pResult->Play(parent, socketName, bFollowParent, location, rotation, scale, onReturn);
		return pResult;
	}

	return nullptr;
}

void FXManager::RetrieveFX(AFXActor* fxActor)
{
	if (fxActor && IsValid(fxActor))
	{
		auto fxContainer = fxActor->GetContainer();
		if (fxContainer == nullptr)
		{
			const auto& fxName = fxActor->GetClass()->GetName();
			if (m_map_FXContainer.Contains(fxName))
			{
				fxContainer = m_map_FXContainer[fxActor->GetClass()->GetName()];
				fxActor->Init(fxContainer);
			}
			else
			{
				fxActor->Destroy();
			}
		}
		
		fxContainer->m_llist_Inactive.AddTail(fxActor->GetNode());
	}
}

FXContainer* FXManager::RegistFX(UClass* fxClass, int nDefaultCount)
{
	auto fxName = fxClass->GetName();

	// Create Or AddRefCount Container
	FXContainer* pResult = nullptr;
	if (m_map_FXContainer.Contains(fxClass->GetName()))
	{
		pResult = m_map_FXContainer[fxName];
		++(pResult->m_nRefCount);
	}
	else
	{
		pResult = new FXContainer();
		pResult->m_StaticClass = fxClass;
		m_map_FXContainer.Add(fxName, pResult);
	}

	// Check Cached Count
	auto nTotalCachedCount = pResult->m_llist_Active.Num() + pResult->m_llist_Inactive.Num();
	if (nTotalCachedCount >= nDefaultCount)
		return pResult;

	// Create And Pooling
	auto nCreate = nDefaultCount - nTotalCachedCount;
	for (int i = 0 ; i < nCreate; ++i)
	{
		auto fxActor = Cast<AFXActor>(Managers::World->GetWorld()->SpawnActor(fxClass));
		fxActor->Init(pResult);
		fxActor->ReturnToManager();

		if (fxActor->IsLooping())
			pResult->m_bIsLoop = true;
		
#if WITH_EDITOR
		fxActor->SetFolderPath(*FString::Printf(TEXT("Actors_FX/%s"), *fxClass->GetName()));
#endif
	}

	return pResult;
}

void FXManager::ReleaseFX(FString fxName)
{
	if(m_map_FXContainer.Contains(fxName))
	{
		auto fxContainer = m_map_FXContainer[fxName];
		if (fxContainer == nullptr)
			return;

		// SubRefCount
		--(fxContainer->m_nRefCount);
		if (fxContainer->m_nRefCount > 0)
			return;

		m_map_FXContainer.Remove(fxName);

		// delete
		delete fxContainer;
	}
}

void FXManager::ReleaseFX(FXContainer* fxContainer)
{	
	if (this == nullptr || fxContainer == nullptr || IsValid(fxContainer->m_StaticClass))
		return;

	// SubRefCount
	--(fxContainer->m_nRefCount);
	if (fxContainer->m_nRefCount > 0)
		return;

	auto fxName = fxContainer->m_StaticClass->GetName();
	if(!fxName.IsEmpty() && m_map_FXContainer.Contains(fxName))
		m_map_FXContainer.Remove(fxName);

	// delete
	delete fxContainer;
}

AFXActor* FXManager::GenerateFX(FXContainer* const fxContainer)
{
	AFXActor* pResult;
	auto node_FxActor = fxContainer->m_llist_Inactive.GetHead();

	if (node_FxActor)
	{
		pResult = node_FxActor->GetValue();
	}
	else
	{
		// 남은 잔량이 없다면 새로 스폰
		pResult = Cast<AFXActor>(Managers::World->GetWorld()->SpawnActor(fxContainer->m_StaticClass));
		pResult->Init(fxContainer);
		node_FxActor = pResult->GetNode();
	}

	fxContainer->m_llist_Active.AddTail(node_FxActor);
	return pResult;
}
