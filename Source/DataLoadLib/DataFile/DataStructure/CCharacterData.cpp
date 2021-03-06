// Generate By DataTool. 2021-12-25 오전 7:02:26
// Drum.

#include "CCharacterData.h"

void CCharacterData::SetInfo(const FRowDataInfo& fInfo)
{
	CDataFileBase::SetInfo(fInfo);

	Name = fInfo.arrColData[1];
	RefPath = fInfo.arrColData[2];
	FileNameModel = fInfo.arrColData[3];
	FileNameAnimInst = fInfo.arrColData[4];
	FileNameAIBT = fInfo.arrColData[5];
	DataIdStat = FCString::Atoi(*fInfo.arrColData[6]);
	Height = FCString::Atoi(*fInfo.arrColData[7]);
	Radius = FCString::Atoi(*fInfo.arrColData[8]);
	Sight = FCString::Atoi(*fInfo.arrColData[9]);
}

EDataFileEnum CCharacterData::GetEnum()
{
	return EDataFileEnum::CharacterData;
}
