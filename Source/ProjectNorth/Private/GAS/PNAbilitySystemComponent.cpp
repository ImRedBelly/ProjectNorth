// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PNAbilitySystemComponent.h"

#include "AudioMixerBlueprintLibrary.h"
#include "PNAttributeSet.h"

UPNAbilitySystemComponent::UPNAbilitySystemComponent()
{
	GetGameplayAttributeValueChangeDelegate(UPNAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ThisClass::HealthUpdated);
}

void UPNAbilitySystemComponent::ApplyInitialEffects()
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	for (auto& EffectClass : InitialEffects)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(EffectClass, 1, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}

void UPNAbilitySystemComponent::GiveInitialAbilities()
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	for (auto& KV : InitialAbilities)
	{
		GiveAbility(FGameplayAbilitySpec(KV.Value, 1, static_cast<int32>(KV.Key), nullptr));
	}

	for (auto& KV : Abilities)
	{
		GiveAbility(FGameplayAbilitySpec(KV.Value, 0, static_cast<int32>(KV.Key), nullptr));
	}
}

void UPNAbilitySystemComponent::ApplyFullStatEffect()
{
	AuthApplyGameplayEffect(FullStatEffect, 1);
}

void UPNAbilitySystemComponent::HealthUpdated(const FOnAttributeChangeData& ChangeData)
{
	if (ChangeData.NewValue <= 0)
	{
		AuthApplyGameplayEffect(DeathEffect, 1);
	}
}

void UPNAbilitySystemComponent::AuthApplyGameplayEffect(TSubclassOf<UGameplayEffect> GameplayEffect, int Level)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	if (GameplayEffect)
	{
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingSpec(GameplayEffect, Level, MakeEffectContext());
		ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
}
