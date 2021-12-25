#include "WorldManager.h"
#include "Framework/Managers/Managers.h"

void WorldManager::InitLevelLandData(const EWorldId& eWorldId)
{
	m_setLoadedLand.Empty();
	m_qLevelExecuteInfo.Empty();
	m_bLoadingLevel = false;

	m_mapLevelLandData.Empty();
	// m_mapSpawnData.Empty();
	m_eWorldID = eWorldId;
	
	if(eWorldId == EWorldId::Intro)
		return;

	auto* mapLandData = Managers::Data->GetMapData<CLandData>();
	for(auto& tuple : *mapLandData)
	{
		auto& pData = tuple.Value;
		if(pData->WorldId != m_eWorldID)
			continue;

		if(!m_mapLevelLandData.Contains(pData->LandName))
		{
			m_mapLevelLandData.Add(pData->LandName, TSet<FString>());
		}
		
		m_mapLevelLandData[pData->LandName].Add(pData->NeighberLandName);
	}

	// auto* mapSpawnData = Managers::Data->GetMapData<CSpawnData>();
	// for(auto& tuple : *mapSpawnData)
	// {
	// 	auto& pData = tuple.Value;
	// 	if(pData->WorldId != m_eWorldID)
	// 		continue;;
	//
	// 	FString& strLandName = pData->LandName;
	//
	// 	if(!m_mapSpawnData.Contains(strLandName))
	// 		m_mapSpawnData.Add(strLandName, TArray<CSpawnData*>());
	// 	
	// 	m_mapSpawnData[pData->LandName].Add(pData);
	// }


	// //Debug Log
	// for(auto& tuple : m_mapLevelLandData)
	// {
	// 	auto& setNeighber = tuple.Value;
	// 	for(auto& strNeighber : setNeighber)
	// 	{
	// 		LOG_ERROR(TEXT("Key[%s] Value[%s]"), *tuple.Key, *strNeighber);
	// 	}		
	// }
}

const bool& WorldManager::IsLevelLoading() const
{
	return m_bLoadingLevel;
}

void WorldManager::LoadLandAsync(const FString& strLandName)
{
	if (strLandName.IsEmpty())
		return;

	// 필요한 랜드 레벨 수집
	TSet<FString> setNewLands;	
	setNewLands.Add(strLandName);

	if(m_mapLevelLandData.Contains(strLandName))
	{
		for(auto& strNeiborLand : m_mapLevelLandData[strLandName])
			setNewLands.Add(strNeiborLand);
	}

	// 현재 로드 되어있는 랜드 중 삭제해야한다면 큐에 추가
	for (auto& strLandLoaded : m_setLoadedLand)
	{
		if (setNewLands.Contains(strLandLoaded))
			continue;

		m_qLevelExecuteInfo.Enqueue(SLevelExeInfo(ELevelExeType::UnloadLand, strLandLoaded));
	}

	// 로드 큐에 추가
	for (auto& strLandBeLoad : setNewLands)
	{
		if (m_setLoadedLand.Contains(strLandBeLoad))
			continue;

		m_qLevelExecuteInfo.Enqueue(SLevelExeInfo(ELevelExeType::LoadLand, strLandBeLoad));
	}

	// 로딩 중이 아니면 로딩 시작
	if (!m_bLoadingLevel)
	{
		m_bLoadingLevel = true;
		LoadCompleteLevel();
	}
}

void WorldManager::SetLevelAsyncEndCallback(std::function<void()> callback)
{
	m_onLoadLandComplete = callback;
}

void WorldManager::LoadCompleteLevel()
{
	SLevelExeInfo sInfo;
	while (!m_qLevelExecuteInfo.IsEmpty())
	{		
		m_qLevelExecuteInfo.Dequeue(sInfo);

		FLatentActionInfo actionInfo;
		actionInfo.UUID = ++m_nUUIDLevelLoad;
		actionInfo.Linkage = 1;
		actionInfo.CallbackTarget = Managers::GameInst;
		actionInfo.ExecutionFunction = "OnCompleteLoadLevel";

		// Load 일 경우
		switch (sInfo.ExeType)
		{
			case ELevelExeType::LoadLand:
				{
					if (m_setLoadedLand.Contains(sInfo.LevelName))
						continue;

					m_setLoadedLand.Add(sInfo.LevelName);
					LOG(TEXT("LoadLand : %s"), *sInfo.LevelName);
					UGameplayStatics::LoadStreamLevel(m_pCurWorld, *sInfo.LevelName, true, false, actionInfo);
				}
				break;
			case ELevelExeType::UnloadLand:
				{
					if (!m_setLoadedLand.Contains(sInfo.LevelName))
						continue;

					m_setLoadedLand.Remove(sInfo.LevelName);
					UGameplayStatics::UnloadStreamLevel(m_pCurWorld, *(sInfo.LevelName), actionInfo, false);
				}
				break;
			default:
				continue;
		}

		return;
	}
	
	m_bLoadingLevel = false;

	// 완료 콜백
	if (m_onLoadLandComplete)
	{
		m_onLoadLandComplete();
		// auto callback = m_onLoadComplete;
		// m_onLoadComplete = nullptr;
		// callback();
	}
}

void WorldManager::CheckAndLoadLand(const AActor* const& target)
{
	/* 지역 체크 */
	// 타겟 유효성 체크 및 레벨 로드 진행중 체크
	if (!target || m_bLoadingLevel)
		return;
	
	FVector vStart = target->GetActorLocation();
	FVector vEnd = vStart - target->GetActorUpVector() * 1000000;
	
	FCollisionQueryParams	params(NAME_None, false, target);
	// params.bReturnPhysicalMaterial = true;

	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByObjectType(hitResult, vStart, vEnd, ECC_TO_BITFIELD(ECollisionChannel::ECC_GameTraceChannel6), params))
	{
		auto pLandActor = hitResult.GetActor();
		if (!m_pCurLandActor)
		{
			m_pCurLandActor = pLandActor;
#if WITH_EDITOR
			LOG_WARNING(TEXT("Current Land : %s"), *pLandActor->GetName())
#endif
		}
		else if (m_pCurLandActor != pLandActor)
		{
			// 현재 랜드랑 다르면 레벨 로드
			m_pCurLandActor = pLandActor;
			Managers::World->LoadLandAsync(pLandActor->GetName());
#if WITH_EDITOR
			LOG_WARNING(TEXT("Current Land : %s"), *pLandActor->GetName())
#endif
		}
	}
}

// void WorldManager::GetSpawnData(TArray<CSpawnData*>& arrLoadSpawnData)
// {
// 	arrLoadSpawnData.Reset();
//
// 	for(auto& strLandName : m_setLoadedLand)
// 	{
// 		if(m_mapSpawnData.Contains(strLandName))
// 		{
// 			for(auto& pData : m_mapSpawnData[strLandName])
// 			{
// 				arrLoadSpawnData.Add(pData);
// 			}
// 		}
// 	}
// }
