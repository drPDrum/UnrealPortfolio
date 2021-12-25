#pragma once

#include "Factories/Factory.h"
#include "BinaryDataFactory.generated.h"

UCLASS()
//UCLASS(hidecategories=Object)
class DATALOADEDITOR_API UBinaryDataFactory : public UFactory
{
	GENERATED_UCLASS_BODY()

private:
	class CBinaryDataRegistry* m_pRegistry;

public:
	/** UFactory interface */
	virtual bool FactoryCanImport(const FString& Filename) override;

protected:
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;	
};