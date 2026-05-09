// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameplayWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ValueGauge.h"
#include "GAS/PNAttributeSet.h"

void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());
	if (AbilitySystemComponent)
	{
		HealthBar->SetAndBoundToGameplayAttribute(
			AbilitySystemComponent,
			UPNAttributeSet::GetHealthAttribute(),
			UPNAttributeSet::GetMaxHealthAttribute());

		ManaBar->SetAndBoundToGameplayAttribute(
			AbilitySystemComponent,
			UPNAttributeSet::GetManaAttribute(),
			UPNAttributeSet::GetMaxManaAttribute());
	}
}
