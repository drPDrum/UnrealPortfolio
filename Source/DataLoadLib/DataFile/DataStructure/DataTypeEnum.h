// Generate By DataTool. 2021-12-25 오전 7:02:22
// Drum
#pragma once

UENUM()
enum class EPlayerSpawnType : uint8
{
	None = 0,
	Fix = 1,
	RandomLand = 2,
};

UENUM()
enum class EWorldId : uint8
{
	None = 0,
	Intro = 1,
	Game = 2,
	InGame = 3,
	All = 99,	//For Spawn All
};

