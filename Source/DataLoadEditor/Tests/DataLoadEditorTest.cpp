#include "DataLoadEditor/Factory/BinaryDataFactory.h"
#include "DataLoadEditor/Registry/CBinaryDataRegistry.h"
#include "Tests/AutomationCommon.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckBinaryDataDiff, "DrumTest.BinaryData.DiffCheck",
	EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool CheckBinaryDataDiff::RunTest(const FString& Parameters)
{
	CBinaryDataRegistry* pReg = CBinaryDataRegistry::Instance();	
	TestNotNull("Registry Not null", pReg);
	TestFalse("File Diff", pReg->InitCheckFileDiff());	
	return true;
}