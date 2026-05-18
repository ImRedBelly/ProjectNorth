// Fill out your copyright notice in the Description page of Project Settings.


#include "Fremework/PNGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"

APlayerController* APNGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* NewPlayerController = Super::SpawnPlayerController(InRemoteRole, Options);

	FGenericTeamId TeamID = GetTeamIDForPlayer(NewPlayerController);
	if (IGenericTeamAgentInterface* NewPlayerTeamInterface = Cast<IGenericTeamAgentInterface>(NewPlayerController))
	{
		NewPlayerTeamInterface->SetGenericTeamId(TeamID);
	}

	NewPlayerController->StartSpot = FindNextStartSpotFortTeam(TeamID);

	return NewPlayerController;
}

FGenericTeamId APNGameMode::GetTeamIDForPlayer(const APlayerController* PlayerController) const
{
	static int PlayerCount = 0;
	++PlayerCount;
	return FGenericTeamId(PlayerCount % 2);
}

AActor* APNGameMode::FindNextStartSpotFortTeam(const FGenericTeamId& TeamID) const
{
	const FName* StartSpotTag = TeamSpartSpotTagMap.Find(TeamID);
	if (!StartSpotTag) return nullptr;

	UWorld* World = GetWorld();
	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		if (It->PlayerStartTag == *StartSpotTag)
		{
			It->PlayerStartTag = FName("Taken");
			return *It;
		}
	}
	return nullptr;
}
