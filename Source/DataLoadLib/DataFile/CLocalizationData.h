// Generate By DataTool. 2021-12-25 오전 7:02:31
// Generated only one time when file is not exists.
// Drum
#pragma once

enum class ELanguage : uint8;

class DATALOADLIB_API CLocalizationData
{
public:
	FString ID;
	FString Korean;
	FString English;
	FString Japanese;

	void SetInfo(const struct FRowDataInfo& fInfo);

	const FString& GetLanguage(int nIndex) const;

	const FString& GetLanguage(ELanguage eLanguage) const;

	static FString KEY_EMPTY;
};
