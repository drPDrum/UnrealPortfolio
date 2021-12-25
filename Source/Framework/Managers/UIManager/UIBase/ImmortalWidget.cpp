#include "ImmortalWidget.h"

UImmortalWidget::UImmortalWidget(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	m_bInitWidgetAnim = false;
}

void UImmortalWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(!m_bInitWidgetAnim)
	{
		m_bInitWidgetAnim = true;

		UWidgetBlueprintGeneratedClass* pWidgetClass = GetWidgetTreeOwningClass();
		
		for (int i = 0; i < pWidgetClass->Animations.Num(); i++) {
     
			FString strName = pWidgetClass->Animations[i]->GetName();
			m_mapWidgetAnim.Add(strName.Replace(TEXT("_INST"), TEXT("")), pWidgetClass->Animations[i]);
		}
	}
}

void UImmortalWidget::OnLevelRemovedFromWorld(ULevel* MovieSceneBlends, UWorld* InWorld)
{
	// Immortal widget : UI를 제거하지 않는 위젯.
	// 레벨 변경 시 UI를 제거하지 않도록 한다.
	// Super::OnLevelRemovedFromWorld(MovieSceneBlends, InWorld);
}

UWidgetAnimation* UImmortalWidget::GetAnimation(const FString& strAnimName)
{
	if(m_mapWidgetAnim.Contains(strAnimName))
		return m_mapWidgetAnim[strAnimName];

	return nullptr;
}
