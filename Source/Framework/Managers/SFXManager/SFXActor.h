// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"

#include "GameFramework/Actor.h"
#include "RuntimeUtils/GenericEx/DoubleLinkedListEx.h"
#include "SFXActor.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSFXReturn, class ASFXActor*, pSFXReturned);

UCLASS()
class FRAMEWORK_API ASFXActor : public AActor
{
	
	GENERATED_BODY()

	TDoubleLinkedListEx<ASFXActor*>::TDoubleLinkedListExNode* m_Node_Pool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* m_AudioComp;

	FOnSFXReturn m_OnReturn;

public:	
	// Sets default values for this actor's properties
	ASFXActor();
	virtual ~ASFXActor() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Play(USoundBase* sound, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation);
	bool Play(USoundBase* sound, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation, const FOnSFXReturn& onReturn);
	void ReturnToManager();

	TDoubleLinkedListEx<ASFXActor*>::TDoubleLinkedListExNode* GetNode();

	bool IsLoop();

protected:
	bool PlaySFX(USoundBase* sound, AActor* parent, const FName& socketName, const bool& bFollowParent, const FVector& location, const FQuat& rotation);

};