#include "ChangeLevelTrigger.h"

#include "Framework/Managers/Managers.h"

AChangeLevelTrigger::AChangeLevelTrigger()
{
	m_pShapeComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(m_pShapeComp);
}

void AChangeLevelTrigger::OnTrigger()
{
	Super::OnTrigger();

	if(Managers::IsInit())
	{
		Managers::World->LoadWorld(m_eLoadWorldID,
			Managers::Data->ConstData->LOADING_COMMON);
	}
}
