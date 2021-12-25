// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GlobalDataStruct.h"
#include "UObject/NoExportTypes.h"
#include "BinaryDataSource.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(BinaryData, Log, All);

UCLASS()
class DATALOADLIB_API UBinaryDataSource : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Category = SourceAsset, VisibleAnywhere)
	int32					m_nRowCount;
	UPROPERTY(Category = SourceAsset, VisibleAnywhere)
	int32					m_nColCount;
	
public:
	// enum class EDataFileEnum : uint8 index value
	UPROPERTY(Category = SourceAsset, VisibleAnywhere)
	int32					DataFileType;
	// Row Data in Excel
	UPROPERTY(Category = SourceAsset, VisibleAnywhere)
	TArray<FRowDataInfo>	TableInfo;
	
#if WITH_EDITORONLY_DATA
	// .bytes 파일 경로
	UPROPERTY(Category = SourceAsset, VisibleAnywhere)
	FString					FilePath;
	// .bytes 파일의 수정 시간. 파일 diff체크에 사용한다.
	UPROPERTY(Category = SourceAsset, VisibleAnywhere)
	int64					ModifyTick;

	// .byte파일 경로를 통해 에디터에서 해당 파일 읽어 PROPERTY에 해당 내용을 넣는다. 
	bool SetDataFromFile(const FString& strFileName);

	FString ToString() const
	{
		FString RetValue = TEXT("================[BinaryData Source]================");

		for (auto value : TableInfo)
		{
			for(auto value2 : value.arrColData)
				RetValue += value2;

			RetValue += TEXT("\n");
		}
		
		return RetValue;
	}
#endif
};
