// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PNAbilitySystemComponent.h"

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
