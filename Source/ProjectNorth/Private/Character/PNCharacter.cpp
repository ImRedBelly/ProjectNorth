// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PNCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GAS/PNAbilitySystemComponent.h"
#include "GAS/PNAttributeSet.h"

APNCharacter::APNCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AbilitySystemComponent = CreateDefaultSubobject<UPNAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UPNAttributeSet>("AttributeSet");
}

void APNCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APNCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* APNCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
