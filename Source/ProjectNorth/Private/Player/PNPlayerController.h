// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PNPlayerController.generated.h"

class APNPlayerCharacter;
/**
 * 
 */
UCLASS()
class APNPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void AcknowledgePossession(APawn* NewPawn) override;
	
private:
	UPROPERTY()
	APNPlayerCharacter* PlayerCharacter;
};
