// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PNGameplayAbility.generated.h"

UCLASS()
class UPNGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UAnimInstance* GetOwnerAnimInstance() const;

	TArray<FHitResult> GetHitResultsFromSweepLocationTargetData(
		const FGameplayAbilityTargetDataHandle& TargetDataHandle,
		float SphereSweepRadius = 30.f,
		bool bIgnoreSelf = true,
		bool bDrawDebug = false) const;
};
