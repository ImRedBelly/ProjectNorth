// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/GA_Combo.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "PNGameplayTags.h"
#include "GameplayTagsManager.h"
#include "Animation/AnimInstance.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

UGA_Combo::UGA_Combo()
{
	AbilityTags.AddTag(PNGameplayTags::Ability_BaseAttack);
	BlockAbilitiesWithTag.AddTag(PNGameplayTags::Ability_BaseAttack);
}

void UGA_Combo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo,
                                const FGameplayEventData* TriggerEventData)
{
	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayComboMontageTask = UAbilityTask_PlayMontageAndWait::
		CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			ComboMontage);
	PlayComboMontageTask->OnBlendOut.AddDynamic(this, &UGA_Combo::K2_EndAbility);
	PlayComboMontageTask->OnCancelled.AddDynamic(this, &UGA_Combo::K2_EndAbility);
	PlayComboMontageTask->OnCompleted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
	PlayComboMontageTask->OnInterrupted.AddDynamic(this, &UGA_Combo::K2_EndAbility);
	PlayComboMontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitComboChangeEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
		PNGameplayTags::Ability_Combo_Change,
		nullptr,
		false,
		false);
	WaitComboChangeEventTask->EventReceived.AddDynamic(this, &ThisClass::ComboChangeEventReceived);
	WaitComboChangeEventTask->ReadyForActivation();

	if (K2_HasAuthority())
	{
		UAbilityTask_WaitGameplayEvent* WaitTargetingEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,
			PNGameplayTags::Ability_Combo_Damage);
		WaitTargetingEventTask->EventReceived.AddDynamic(this, &ThisClass::DoDamage);
		WaitTargetingEventTask->ReadyForActivation();
	}

	SetupWaitComboPress();
}

void UGA_Combo::ComboChangeEventReceived(FGameplayEventData Payload)
{
	FGameplayTag EventTag = Payload.EventTag;
	if (EventTag == PNGameplayTags::Ability_Combo_Change_End)
	{
		NextComboName = NAME_None;
		return;
	}

	TArray<FName> TagNames;
	UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
	NextComboName = TagNames.Last();
}

void UGA_Combo::SetupWaitComboPress()
{
	UAbilityTask_WaitInputPress* WaitInputPress = UAbilityTask_WaitInputPress::WaitInputPress(this);
	WaitInputPress->OnPress.AddDynamic(this, &ThisClass::HandleInputPress);
	WaitInputPress->ReadyForActivation();
}

void UGA_Combo::HandleInputPress(float TimeWaited)
{
	SetupWaitComboPress();
	TryCommitCombo();
}

void UGA_Combo::DoDamage(FGameplayEventData Payload)
{
	TArray<FHitResult> HitResults = GetHitResultsFromSweepLocationTargetData(
		Payload.TargetData, TargetSweepSphereRadius, ETeamAttitude::Hostile, true, bDebug);
	for (const FHitResult& HitResult : HitResults)
	{
		TSubclassOf<UGameplayEffect> DamageEffect = GetDamageEffectForCurrentCombo();
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(
			DamageEffect,
			GetAbilityLevel(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo()));

		FGameplayEffectContextHandle EffectContext = MakeEffectContext(GetCurrentAbilitySpecHandle(),
		                                                               GetCurrentActorInfo());
		EffectContext.AddHitResult(HitResult);
		EffectSpecHandle.Data->SetContext(EffectContext);

		ApplyGameplayEffectSpecToTarget(
			GetCurrentAbilitySpecHandle(),
			CurrentActorInfo,
			CurrentActivationInfo,
			EffectSpecHandle,
			UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(HitResult.GetActor()));
	}
}


void UGA_Combo::TryCommitCombo() const
{
	if (NextComboName == NAME_None) return;
	if (UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance())
	{
		OwnerAnimInstance->Montage_SetNextSection(
			OwnerAnimInstance->Montage_GetCurrentSection(ComboMontage),
			NextComboName,
			ComboMontage);
	}
}

TSubclassOf<UGameplayEffect> UGA_Combo::GetDamageEffectForCurrentCombo() const
{
	if (const UAnimInstance* OwnerAnimInstance = GetOwnerAnimInstance())
	{
		const FName CurrentSectionName = OwnerAnimInstance->Montage_GetCurrentSection(ComboMontage);
		if (DamageEffectMap.Contains(CurrentSectionName))
		{
			return DamageEffectMap[CurrentSectionName];
		}
	}
	return DefaultDamageEffect;
}
