// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PNPlayerController.h"

#include "PNPlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/GameplayWidget.h"

void APNPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayerCharacter = Cast<APNPlayerCharacter>(InPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->ServerSideInit();
		PlayerCharacter->SetGenericTeamId(TeamID);
	}
}

void APNPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);
	PlayerCharacter = Cast<APNPlayerCharacter>(NewPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->ClientSideInit();
		SpawnGameplayWidget();
	}
}

void APNPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APNPlayerController, TeamID);
}

FGenericTeamId APNPlayerController::GetGenericTeamId() const
{
	return TeamID;
}

void APNPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

void APNPlayerController::SpawnGameplayWidget()
{
	if (!IsLocalPlayerController()) return;

	GameplayWidget = CreateWidget<UGameplayWidget>(GetWorld(), GameplayWidgetClass);
	if (GameplayWidget)
	{
		GameplayWidget->AddToViewport();
	}
}
