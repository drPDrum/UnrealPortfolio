#pragma once
#include "DataLoadLib/DataLoadLibInfo.h"

DECLARE_LOG_CATEGORY_EXTERN(DLE, Log, All);
#define	LOGDLE_CALLINFO					(FString(__FUNCTION__) + TEXT(":Line.") + FString::FromInt(__LINE__))
#define	LOGDLE(Format, ...)				UE_LOG(DLE, Display, TEXT("%s : %s"), *LOGDLE_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define	LOGDLE_WARNING(Format, ...)		UE_LOG(DLE, Warning, TEXT("%s : %s"), *LOGDLE_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define	LOGDLE_ERROR(Format, ...)		UE_LOG(DLE, Error, TEXT("%s : %s"), *LOGDLE_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))