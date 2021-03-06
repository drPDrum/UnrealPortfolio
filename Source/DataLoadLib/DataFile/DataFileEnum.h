// Generate By DataTool. 2021-12-25 오전 7:02:31
// Drum
#pragma once

UENUM()
enum class EDataFileEnum : uint8
{
	ConstData = 0,
	CharacterData = 1,
	ItemData = 2,
	HelpData = 3,
	HelpCategoryData = 4,
	LandData = 5,
	StatData = 6,
	MAX,
};

UENUM()
enum class ELanguage : uint8
{
	None = 0,
	Korean = 1,
	English = 2,
	Japanese = 3,
};
