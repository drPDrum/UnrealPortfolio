// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Framework/CommonInfo.h"
#include "EmptyBoxCollider.generated.h"

UCLASS()
class FRAMEWORK_API AEmptyBoxCollider : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* m_pBoxComponent;
	
public:	
	// Sets default values for this actor's properties
	AEmptyBoxCollider();
};
