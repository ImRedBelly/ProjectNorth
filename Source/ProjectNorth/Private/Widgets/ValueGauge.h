// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Blueprint/UserWidget.h"
#include "ValueGauge.generated.h"

struct FGameplayAttribute;
class UAbilitySystemComponent;
class UTextBlock;
class UProgressBar;

UCLASS()
class UValueGauge : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetValue(float CurrentValue, float MaxValue);
	void SetAndBoundToGameplayAttribute(
		UAbilitySystemComponent* AbilitySystemComponent,
		const FGameplayAttribute& Attribute,
		const FGameplayAttribute& MaxAttribute);

protected:
	virtual void NativePreConstruct() override;

private:
	void ValueChanged(const FOnAttributeChangeData& ChangeData);
	void MaxValueChanged(const FOnAttributeChangeData& ChangeData);

	UPROPERTY(EditAnywhere, Category="Visual")
	FLinearColor BarColor;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UTextBlock* ValueText;

	float CachedCurrentValue;
	float CachedMaxValue;
};
