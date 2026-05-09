// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/ValueGauge.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UValueGauge::SetValue(float CurrentValue, float MaxValue)
{
	CachedCurrentValue = CurrentValue;
	CachedMaxValue = MaxValue;
	if (CurrentValue == 0) return;
	ProgressBar->SetPercent(CurrentValue / MaxValue);

	FNumberFormattingOptions Options = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	ValueText->SetText(
		FText::Format(
			FTextFormat::FromString("{0}/{1}"),
			FText::AsNumber(CurrentValue, &Options),
			FText::AsNumber(MaxValue, &Options)));
}

void UValueGauge::SetAndBoundToGameplayAttribute(UAbilitySystemComponent* AbilitySystemComponent,
                                                 const FGameplayAttribute& Attribute,
                                                 const FGameplayAttribute& MaxAttribute)
{
	if (AbilitySystemComponent)
	{
		bool bFound;
		bool bFoundMax;
		float Value = AbilitySystemComponent->GetGameplayAttributeValue(Attribute, bFound);
		float MaxValue = AbilitySystemComponent->GetGameplayAttributeValue(MaxAttribute, bFoundMax);

		if (bFound && bFoundMax)
		{
			SetValue(Value, MaxValue);
		}
	}
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
	                      .AddUObject(this, &UValueGauge::ValueChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute)
	                      .AddUObject(this, &UValueGauge::MaxValueChanged);
}

void UValueGauge::NativePreConstruct()
{
	Super::NativePreConstruct();
	ProgressBar->SetFillColorAndOpacity(BarColor);
}

void UValueGauge::ValueChanged(const FOnAttributeChangeData& ChangeData)
{
	SetValue(ChangeData.NewValue, CachedMaxValue);
}

void UValueGauge::MaxValueChanged(const FOnAttributeChangeData& ChangeData)
{
	SetValue(CachedCurrentValue, ChangeData.NewValue);
}
