// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/GameModeBase.h"
#include "PNGameMode.generated.h"

UCLASS()
class APNGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, const FString& Options) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Teams")
	TMap<FGenericTeamId, FName> TeamSpartSpotTagMap;
	
	FGenericTeamId GetTeamIDForPlayer(const APlayerController* PlayerController) const;
	AActor* FindNextStartSpotFortTeam(const FGenericTeamId& TeamID) const;
};
