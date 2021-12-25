#pragma once
#include "EngineMinimal.h"
#include "GlobalDataStruct.generated.h"

UENUM()
enum class EDataType : uint8
{
	NONE = 0, INT, FLOAT, STRING, LONG, ENUM, BOOL,MAX
};

USTRUCT()
struct FRowDataInfo
{
	GENERATED_BODY()
	
	UPROPERTY(Category = SourceAsset, VisibleAnywhere)
	TArray<FString>	arrColData;
};