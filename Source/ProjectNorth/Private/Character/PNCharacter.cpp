// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PNCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GAS/PNAbilitySystemComponent.h"
#include "GAS/PNAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/OverHeadStatsGauge.h"

APNCharacter::APNCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AbilitySystemComponent = CreateDefaultSubobject<UPNAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UPNAttributeSet>("AttributeSet");

	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Over Head Widget Component");
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());
}

void APNCharacter::ServerSideInit()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->ApplyInitialEffects();
}

void APNCharacter::ClientSideInit()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

bool APNCharacter::IsLocallyControllerByPlayer() const
{
	return GetController() && GetController()->IsLocalPlayerController();
	// return GetLocalRole() == ROLE_AutonomousProxy || GetRemoteRole() == ROLE_AutonomousProxy;
}


void APNCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigureOverHeadStatusWidget();
}

void APNCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APNCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (NewController && !NewController->IsPlayerController())
	{
		ServerSideInit();
	}
}

UAbilitySystemComponent* APNCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APNCharacter::ConfigureOverHeadStatusWidget()
{
	if (!OverHeadWidgetComponent) return;

	if (IsLocallyControllerByPlayer())
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
		return;
	}

	if (UOverHeadStatsGauge* OverHeadStatsGauge = Cast<UOverHeadStatsGauge>(
		OverHeadWidgetComponent->GetUserWidgetObject()))
	{
		OverHeadStatsGauge->ConfigureWithACS(AbilitySystemComponent);
		OverHeadWidgetComponent->SetHiddenInGame(false);
		GetWorldTimerManager().ClearTimer(HeadStatGaugeVisibilityUpdateTimerHandle);
		GetWorldTimerManager().SetTimer(HeadStatGaugeVisibilityUpdateTimerHandle, this,
		                                &ThisClass::UpdateHeadStatGaugeVisibility,
		                                HeadStatGaugeVisibilityCheckUpdateGap, true);
	}
}

void APNCharacter::UpdateHeadStatGaugeVisibility() const
{
	if (const APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		const float DistanceSquared = FVector::DistSquared(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
		OverHeadWidgetComponent->SetHiddenInGame(DistanceSquared > HeadStatGaugeVisibilityRangeSquared);
	}
}
