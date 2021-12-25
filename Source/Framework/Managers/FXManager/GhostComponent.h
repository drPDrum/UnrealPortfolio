// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "Components/SceneComponent.h"
#include "GhostComponent.generated.h"

class UPoseableMeshComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnGhostFinished, class UGhostComponent*, PSystem );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FRAMEWORK_API UGhostComponent : public USceneComponent
{
	GENERATED_BODY()

protected:	
	struct MatContainer
	{
		int RefCount;
		TArray<UMaterialInstanceDynamic*> Materials;
	};

	enum class EGhostState : uint8
	{
		Playing,
		Release,
		Stopped,
	};

	static TMap<uint32, MatContainer> map_Materials;

	/* 디자이너 노출부 */
	// 사용할 머티리얼
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* m_MatFX;
	// 머티리얼 내의 투명도를 조절할 파라미터 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FString m_OpacityParamName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	// 선명하게 살아있는 시간
	float m_fLiveTime  = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	// Return 이후에 소멸 시간
	float m_fFadeTime  = 1;

	/* 코드에서만 사용 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UPoseableMeshComponent*>	m_PoseableMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*>	m_StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInstanceDynamic*> m_UseMat_Poseable;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInstanceDynamic*> m_UseMat_Static;

	MatContainer* m_pMatFxContainer;
	EGhostState m_State;
	
	int m_nUseCount_PoseableMesh;
	int m_nUseCount_StaticMesh;
	float m_fRemainTime;


public:
	FOnGhostFinished OnGhostFinished;

public:	
	// Sets default values for this component's properties
	UGhostComponent();

public:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:	
	void ClearAllMeshComp();
	void ClearPoseableMeshComp();
	void ClearStaticMeshComp();

public:
	void SetTarget(const TArray<USkeletalMeshComponent*>& skeletalMeshComps, const TArray<UStaticMeshComponent*>& staticMeshComps);
	void SetTarget(const TArray<USkeletalMeshComponent*>& skeletalMeshComps);
	void SetTarget(const TArray<UStaticMeshComponent*>& staticMeshComps);
	
	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	void DeactivateImmediate();

	bool IsLooping() const;
};
