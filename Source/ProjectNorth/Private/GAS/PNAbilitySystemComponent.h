// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PNGameplayAbilityTypes.h"
#include "PNAbilitySystemComponent.generated.h"

UCLASS()
class UPNAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UPNAbilitySystemComponent();

	void ApplyInitialEffects();
	void GiveInitialAbilities();
	void ApplyFullStatEffect();

private:
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effects")
	TArray<TSubclassOf<UGameplayEffect>> InitialEffects;

	UPROPERTY(EditDefaultsOnly, Category="Initial Abilities")
	TMap<EAbilityInputID, TSubclassOf<UGameplayAbility>> InitialAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Initial Abilities")
	TMap<EAbilityInputID, TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effects")
	TSubclassOf<UGameplayEffect> DeathEffect;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effects")
	TSubclassOf<UGameplayEffect> FullStatEffect;

	void HealthUpdated(const FOnAttributeChangeData& ChangeData);

	void AuthApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int Level);
};
