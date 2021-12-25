#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"

DECLARE_LOG_CATEGORY_EXTERN(Framework, Log, All);
#define	LOG_CALLINFO				(FString(__FUNCTION__) + TEXT(":Line.") + FString::FromInt(__LINE__))
#define	LOG(Format, ...)			UE_LOG(Framework, Display, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define	LOG_WARNING(Format, ...)	UE_LOG(Framework, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define	LOG_ERROR(Format, ...)		UE_LOG(Framework, Error, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

void PrintViewport(float fTime, const FColor& sColor, const FString& strText);