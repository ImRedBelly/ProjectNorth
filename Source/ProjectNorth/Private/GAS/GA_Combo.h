// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNGameplayAbility.h"
#include "GA_Combo.generated.h"

UCLASS()
class UGA_Combo : public UPNGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Combo();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Debug")
	bool bDebug = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Targetting")
	float TargetSweepSphereRadius = 30.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* ComboMontage;

	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effect")
	TSubclassOf<UGameplayEffect> DefaultDamageEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Effect")
	TMap<FName, TSubclassOf<UGameplayEffect>> DamageEffectMap;

	UFUNCTION()
	void ComboChangeEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void SetupWaitComboPress();

	UFUNCTION()
	void HandleInputPress(float TimeWaited);

	UFUNCTION()
	void DoDamage(FGameplayEventData Payload);

	void TryCommitCombo() const;
	TSubclassOf<UGameplayEffect> GetDamageEffectForCurrentCombo() const;

	FName NextComboName;
};
