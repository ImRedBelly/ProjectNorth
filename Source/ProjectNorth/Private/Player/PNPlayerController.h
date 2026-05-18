// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PNPlayerController.generated.h"

class APNPlayerCharacter;

UCLASS()
class APNPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AcknowledgePossession(APawn* NewPawn) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;

private:
	UPROPERTY()
	APNPlayerCharacter* PlayerCharacter;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> GameplayWidgetClass;

	UPROPERTY()
	UUserWidget* GameplayWidget;

	UPROPERTY(Replicated)
	FGenericTeamId TeamID;

	void SpawnGameplayWidget();
};
