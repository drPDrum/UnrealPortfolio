// Fill out your copyright notice in the Description page of Project Settings.

#include "BinaryDataSource.h"

DEFINE_LOG_CATEGORY(BinaryData);

#if WITH_EDITORONLY_DATA
bool UBinaryDataSource::SetDataFromFile(const FString& strFileName)
{	
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();	
	IFileHandle* fileHandle = platformFile.OpenRead(*strFileName);
	
	if(fileHandle)
	{
		FilePath = strFileName;
		FFileStatData sFileStat = IFileManager::Get().GetStatData(*strFileName);
		ModifyTick = sFileStat.ModificationTime.GetTicks();
		
		uint8* pBuffer = reinterpret_cast<uint8*>(&DataFileType);
		fileHandle->Read(pBuffer, sizeof(int32));
		
		pBuffer = reinterpret_cast<uint8*>(&m_nRowCount);		
		fileHandle->Read(pBuffer, sizeof(int32));
		
		pBuffer = reinterpret_cast<uint8*>(&m_nColCount);
		fileHandle->Read(pBuffer, sizeof(int32));
		
		TableInfo.Reset();
		
		for(int nRow = 0 ; nRow < m_nRowCount ; ++nRow)
		{
			FRowDataInfo fRowInfo;

			TableInfo.Add(fRowInfo);
		}	

		for(int nCol = 0 ; nCol < m_nColCount ; ++nCol)
		{
			int32 nValue = 0;

			pBuffer = reinterpret_cast<uint8*>(&nValue);
			fileHandle->Read(pBuffer, sizeof(int32));

			if(nValue == 0)
				continue;
			
			EDataType eType = static_cast<EDataType>(nValue);

			if(nCol == 0)
			{
				if(eType != EDataType::INT && eType != EDataType::STRING)
				{
					UE_LOG(BinaryData, Error, TEXT("Key should be int or String"));
					continue;
				}
			}
			
			for(int nRow = 0 ; nRow < m_nRowCount ; ++nRow)
			{
				TArray<FString> arrRow = TableInfo[nRow].arrColData;
				
				switch (eType)
				{
				case EDataType::INT:
				case EDataType::ENUM:
				case EDataType::BOOL:
					{
						nValue = 0;
						pBuffer = reinterpret_cast<uint8*>(&nValue);
						fileHandle->Read(pBuffer, sizeof(int32));

						arrRow.Add(FString::FromInt(nValue));							
					}
					break;
				case EDataType::FLOAT:
					{
						float fValue = 0.f;					
						pBuffer = reinterpret_cast<uint8*>(&fValue);

						fileHandle->Read(pBuffer, sizeof(float));
						arrRow.Add(FString::Printf(TEXT("%f"), fValue));	
					}					
					break;
				case EDataType::STRING:
					{
						//String Size
						nValue = 0;
						pBuffer = reinterpret_cast<uint8*>(&nValue);
						fileHandle->Read(pBuffer, sizeof(int32));

						char Buffer2[2048];					

						pBuffer = reinterpret_cast<uint8*>(&Buffer2);
						fileHandle->Read(pBuffer, nValue);

						Buffer2[nValue] = '\0';
							Buffer2[nValue+1] = '\0';

						FString strValue = UTF16_TO_TCHAR(Buffer2);
						arrRow.Add(strValue);						
					}
					break;

				case EDataType::LONG:
					{
						int64 lValue = 0;
						pBuffer = reinterpret_cast<uint8*>(&lValue);

						fileHandle->Read(pBuffer, sizeof(int64));
						arrRow.Add(FString::Printf(TEXT("%lld"), lValue));	
					}
					break;					
				}

				TableInfo[nRow].arrColData = arrRow;
			}	
		}
		
		delete fileHandle;

		UE_LOG(BinaryData, Log, TEXT("Load BinaryData File Success! : %s"), *strFileName);

		return true;
	}
	else
	{
		UE_LOG(BinaryData, Error, TEXT("Load BinaryData File Fail! : %s"), *strFileName);

		return false;
	}
}
#endif
