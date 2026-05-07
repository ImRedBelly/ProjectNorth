// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PNPlayerController.h"

#include "PNPlayerCharacter.h"

void APNPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayerCharacter = Cast<APNPlayerCharacter>(InPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->ServerSideInit();
	}
}

void APNPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);
	PlayerCharacter = Cast<APNPlayerCharacter>(NewPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->ClientSideInit();
	}
}
