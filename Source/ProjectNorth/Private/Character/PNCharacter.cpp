// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PNCharacter.h"
#include "Components/SkeletalMeshComponent.h"

APNCharacter::APNCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APNCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APNCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
