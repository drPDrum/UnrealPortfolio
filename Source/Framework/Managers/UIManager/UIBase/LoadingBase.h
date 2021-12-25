#pragma once

#include "ImmortalWidget.h"
#include "LoadingBase.generated.h"

UCLASS()
class FRAMEWORK_API ULoadingBase : public UImmortalWidget
{
	GENERATED_BODY()

public:	
	virtual void Open();
	virtual void Close();

public:
	UFUNCTION(BlueprintCallable)
	void OnEndAnim_Open() const;
	UFUNCTION(BlueprintCallable)
	void OnEndAnim_Close() const;
};