// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PNGameplayAbilityTypes.generated.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	None,
	BaseAttack,
	AbilityOne,
	AbilityTwo,
	AbilityThree,
	AbilityFour,
	AbilityFive,
	AbilitySix,
	Confirm,
	Cancel,
};
