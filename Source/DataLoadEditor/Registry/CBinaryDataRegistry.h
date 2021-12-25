#pragma once

struct DATALOADEDITOR_API SBinaryInfo
{
	int64 lModifyTick;
	//Contents path
	FString strBinaryAssetName;
};

class UBinaryDataFactory;

class DATALOADEDITOR_API CBinaryDataRegistry
{
private:
	UBinaryDataFactory* m_pFactory;

	CBinaryDataRegistry(UBinaryDataFactory* pFactory);	
	~CBinaryDataRegistry();

	// initial file diff check---------------------------
private:	
	// Key : byte file absolute path
	TMap<FString, int64> m_mapTempBinaryModifyData;
	// Key : byte file absolute path	
	TMap<FString, SBinaryInfo> m_mapTempUaastBinaryData;

public:
	bool InitCheckFileDiff();
	void MakeCheckFiles(FString strFileName, const FFileStatData& StatData, int nExtentionIndex);
	bool CheckFilesDiff();
	
	// end initial file diff check-----------------------

	// Directory Watcher --------------------------------
private:
	// for watching the Game directory
	FDelegateHandle GameDirectoryWatcherHandle;

	void OnDirectoryChanged(const TArray<struct FFileChangeData>& inFileChanges);
	
	// Directory Watcher --------------------------------

	// Tick ---------------------------------------------
private:
	FDelegateHandle m_TickDelegateHandle;
	
	void MakeTick();
	bool OnTick(float fDeltaTime);
	void RemoveTick();

	// End Tick -----------------------------------------

	// registry -----------------------------------------
private:	
	TArray<FString> m_arrRegistyFileName;
	TArray<TPair<FString, FString>> m_arrRegistryFilesAndDest;

public:
	void RegistryAsset();
	bool GetContentsPathFromAbsolutePath(const FString& strAbsolutePath, FString& strOutString);
	// end registry -------------------------------------

	// +Singleton.cpp------------------------------------
private:
	static CBinaryDataRegistry* m_instance;
public:
	static CBinaryDataRegistry* Instance(UBinaryDataFactory* pFactory);
	static CBinaryDataRegistry* Instance();
	static void Init();
	// End Singleton-------------------------------------
};
