// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PNCharacter.generated.h"

UCLASS()
class APNCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APNCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
