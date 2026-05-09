// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/OverHeadStatsGauge.h"

#include "ValueGauge.h"
#include "GAS/PNAttributeSet.h"

void UOverHeadStatsGauge::ConfigureWithACS(UAbilitySystemComponent* AbilitySystemComponent)
{
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
