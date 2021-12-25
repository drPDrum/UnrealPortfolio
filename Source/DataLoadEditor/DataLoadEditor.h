#include "IAssetTypeActions.h"
#include "Modules/ModuleManager.h"

class FDataLoadEditor : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
};