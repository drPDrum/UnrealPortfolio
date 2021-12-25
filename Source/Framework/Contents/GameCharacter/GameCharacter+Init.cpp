#include "GameCharacter.h"

#include "ActionControl/ActionController.h"
#include "AnimControl/AnimController.h"
#include "BrainControl/BrainController.h"
#include "StatControl/StatController.h"

#include "Framework/Managers/Managers.h"
#include "Framework/UserInfo/UserData/CUserCharData.h"
//#include "Framework/Managers/GameDataManager/GameDataManager.h"
//#include "Client/GameContents/GameDatas/GameDataStructs.h"

void AGameCharacter::Init(CUserCharData* pUserData)
{
	if (!pUserData)
		return;

	// Init Controllers
	if (!m_ActionCtrl)
	{
		m_ActionCtrl = NewObject<UActionController>(this);
		m_ActionCtrl->Init(this);
	}
	
	if (!m_AnimCtrl)
	{
		m_AnimCtrl =  NewObject<UAnimController>(this);
		m_AnimCtrl->Init(this);
	}
	
	if (!m_StatCtrl)
	{
		m_StatCtrl = NewObject<UStatController>(this);
		m_StatCtrl->Init(this);
	}
	
	if (!m_BrainCtrl)
	{
		m_BrainCtrl = Cast<ABrainController>(GetController());
		// Init은 데이터 세팅 이후에 한다.
		//m_BrainCtrl->Init(this);
	}

	SetActorTickEnabled(true);
	
	// Init Data
	m_pUserData = pUserData;
	auto pData = m_pUserData->GetData();
	auto halfHeight = pData->Height * 0.5f;
	auto pCapsuleComp = GetCapsuleComponent();

	// Init Collision
	pCapsuleComp->SetCapsuleHalfHeight(halfHeight);
	pCapsuleComp->SetCapsuleRadius(pData->Radius);

	auto pMeshComp = GetMesh();
	pMeshComp->SetRelativeLocation(FVector::DownVector * halfHeight);

	// Todo :: 예시 메쉬의 유격 보정
	pMeshComp->SetRelativeRotation(FQuat(FVector::UpVector, FMath::DegreesToRadians(-90.f)));
	
	// Init Mesh
	auto pathFile_SkeletonMesh = FPaths::Combine(pData->RefPath, TEXT("Skins"), pData->FileNameModel);
   	auto pathRef_SkeletonMesh = FString::Printf(TEXT("SkeletalMesh'%s.%s'"), *pathFile_SkeletonMesh, *(pData->FileNameModel));

	auto skelMesh = Managers::Asset->LoadObj<USkeletalMesh>(*pathRef_SkeletonMesh);
	if (skelMesh)
		GetMesh()->SetSkeletalMesh(skelMesh);
	
	// Init BlueprintLoa
	auto pathFile_AnimBlueprint = FPaths::Combine(pData->RefPath, TEXT("Animations"), pData->FileNameAnimInst);
   	auto pathRef_AnimBlueprint = FString::Printf(TEXT("AnimBlueprint'%s.%s_C'"), *pathFile_AnimBlueprint, *(pData->FileNameAnimInst));

	const auto animBlueprint = Managers::Asset->LoadBP<UAnimInst>(*pathRef_AnimBlueprint);
	if (animBlueprint)
		m_AnimCtrl->SetAnimInst(animBlueprint);

	// Init ActionData
	TArray<UActionData*> actionDatas;
	Managers::Data->GetActionDatas(pData->ID, actionDatas);
	m_ActionCtrl->InitActionDatas(actionDatas);

	// Init AI
	m_BrainCtrl->Init(this);

	RefreshName();
}

void AGameCharacter::RefreshName()
{
#if WITH_EDITOR
	auto teamInfo = GetTeamInfo();	
	if (teamInfo && m_pUserData)
		SetActorLabel(*FString::Printf(TEXT("GameCha_%03d_%s"), teamInfo->GetIdx(), *m_pUserData->GetData()->Name));
	else if (teamInfo)
		SetActorLabel(*FString::Printf(TEXT("GameCha_%03d"), teamInfo->GetIdx()));
	else if (m_pUserData)
		SetActorLabel(*FString::Printf(TEXT("GameCha_%s"), *m_pUserData->GetData()->Name));
	else
		SetActorLabel(TEXT("GameCha"));
#endif
}
