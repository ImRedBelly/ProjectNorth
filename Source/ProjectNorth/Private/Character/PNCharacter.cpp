// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/PNCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/PNAbilitySystemComponent.h"
#include "GAS/PNAttributeSet.h"
#include "GAS/PNGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/OverHeadStatsGauge.h"

APNCharacter::APNCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AbilitySystemComponent = CreateDefaultSubobject<UPNAbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UPNAttributeSet>("AttributeSet");

	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Over Head Widget Component");
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());

	BindGASChangeDelegated();
}

void APNCharacter::ServerSideInit()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->ApplyInitialEffects();
	AbilitySystemComponent->GiveInitialAbilities();
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

FGenericTeamId APNCharacter::GetGenericTeamId() const
{
	return TeamID;
}

void APNCharacter::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

void APNCharacter::BeginPlay()
{
	Super::BeginPlay();
	ConfigureOverHeadStatusWidget();

	MeshRelativeTransform = GetMesh()->GetRelativeTransform();
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

void APNCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APNCharacter, TeamID);
}

UAbilitySystemComponent* APNCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APNCharacter::OnDeath()
{
}

void APNCharacter::OnRespawn()
{
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

void APNCharacter::SetStatusGaugeEnable(bool bIsEnable)
{
	GetWorldTimerManager().ClearTimer(HeadStatGaugeVisibilityUpdateTimerHandle);

	if (bIsEnable)
	{
		ConfigureOverHeadStatusWidget();
	}
	else
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
	}
}

void APNCharacter::BindGASChangeDelegated()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(PNGameplayTags::Stats_Death)
		                      .AddUObject(this, &ThisClass::DeathTagUpdated);
	}
}

void APNCharacter::DeathTagUpdated(const FGameplayTag Tag, int NewCount)
{
	if (NewCount == 0)
	{
		Respawn();
	}
	else
	{
		StartDeathSequence();
	}
}

void APNCharacter::StartDeathSequence()
{
	OnDeath();
	SetStatusGaugeEnable(false);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	float Duration = PlayAnimMontage(DeathMontage);
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ThisClass::DeathFinished,
	                                Duration + DeathMontageFinishTimeShift, false);
}

void APNCharacter::Respawn()
{
	OnRespawn();
	SetRagdollEnable(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	SetStatusGaugeEnable(true);

	if (HasAuthority() && GetController())
	{
		TWeakObjectPtr<AActor> StartSpot = GetController()->StartSpot;
		if (StartSpot.IsValid())
		{
			SetActorTransform(StartSpot->GetActorTransform());
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->ApplyFullStatEffect();
	}
}

void APNCharacter::DeathFinished()
{
	SetRagdollEnable(true);
}

void APNCharacter::SetRagdollEnable(bool bIsEnable)
{
	if (bIsEnable)
	{
		GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetMesh()->SetSimulatePhysics(bIsEnable);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		GetMesh()->SetRelativeTransform(MeshRelativeTransform);
	}
}
