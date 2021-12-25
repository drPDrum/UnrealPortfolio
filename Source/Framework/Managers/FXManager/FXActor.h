// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "GameFramework/Actor.h"
#include "RuntimeUtils/GenericEx/DoubleLinkedListEx.h"
#include "FXActor.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFXReturn, class AFXActor*, pFXReturned);

class FXContainer;

enum class EFXStateTypes
{
	Playing,
	WaitForReturn,
	Stopped,
};

class UNiagaraComponent;
class UGhostComponent;

UCLASS()
class FRAMEWORK_API AFXActor : public AActor
{
	GENERATED_BODY()

protected:

	TDoubleLinkedListEx<AFXActor*>::TDoubleLinkedListExNode* m_Node_Pool;
	FXContainer* m_pContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UMeshComponent*> m_Meshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UParticleSystemComponent*> m_Particles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UNiagaraComponent*> m_Niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UGhostComponent*> m_Ghost;

	EFXStateTypes m_State;
	bool m_bIsLoop;
	int m_nParticleCount;
	int m_nRemainParticleCount;

	FOnFXReturn m_OnReturn;

public:	
	// Sets default values for this actor's properties
	AFXActor();
	virtual ~AFXActor() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Init(FXContainer* myContainer);

	bool Play(AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale);
	bool Play(AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale, const FOnFXReturn& onReturn);
	virtual void Stop();
	virtual void ReturnToManager();

	TDoubleLinkedListEx<AFXActor*>::TDoubleLinkedListExNode* GetNode() const;
	FXContainer* GetContainer();

	bool IsLooping();
	bool HasGhost();
	
	void SetGhost(const TArray<USkeletalMeshComponent*>& skeletalMeshComps, const TArray<UStaticMeshComponent*>& staticMeshComps);
	void SetGhost(const TArray<USkeletalMeshComponent*>& skeletalMeshComps);
	void SetGhost(const TArray<UStaticMeshComponent*>& staticMeshComps);

	UFUNCTION()
	void OnParticleSystemFinished(UParticleSystemComponent* pParticle);
	UFUNCTION()
	void OnNiagaraSystemFinished(UNiagaraComponent* pParticle);
	UFUNCTION()
    void OnGhostFinished(UGhostComponent* pGhost);

protected :
	void CheckAllComponentEnd();
	virtual bool PlayFX(AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FVector& scale);
};

