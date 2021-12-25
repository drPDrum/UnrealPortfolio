#include "CommonInfo.h"

DEFINE_LOG_CATEGORY(Framework);

void PrintViewport(float fTime, const FColor& sColor, const FString& strText)
{
	GEngine->AddOnScreenDebugMessage(-1, fTime, sColor, strText);
}
