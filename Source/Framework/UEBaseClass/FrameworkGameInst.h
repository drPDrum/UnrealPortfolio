// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../CommonInfo.h"
#include "FrameworkGameInst.generated.h"

UCLASS()
class FRAMEWORK_API UFrameworkGameInst : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TSet<UUserWidget*>			m_setWidgetInst;	
	UPROPERTY()
	TSet<UMaterialInterface*>	m_setMaterial;
	
	virtual void Init() override;
	virtual void BeginDestroy() override;


public:
	//Widget
	void AddWidget(UUserWidget* pUserWidget);
	void RemoveWidget(UUserWidget* pUserWidget);
	void ClearWidgets();
	
	//Material
	void AddMat(UMaterialInterface* pMat);
	void RemoveMat(UMaterialInterface* pMat);
	void RemoveMat(const TArray<UMaterialInterface*>& mats);
	void RemoveMat(const TArray<UMaterialInstanceDynamic*>& mats);
	void ClearMat();

public: // Callback For Managers
	UFUNCTION(BlueprintCallable)
	void OnCompleteLoadLevel();
};