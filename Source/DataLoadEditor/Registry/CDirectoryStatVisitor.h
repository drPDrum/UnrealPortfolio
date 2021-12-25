#pragma once

class DATALOADEDITOR_API CDirectoryStatVisitor : public IPlatformFile::FDirectoryStatVisitor
{
private:	
	const TArray<FString> m_arrExtention;
	
public:
	CDirectoryStatVisitor();
	
protected:
	virtual bool Visit(const TCHAR* FilenameOrDirectory, const FFileStatData& StatData) override;
};