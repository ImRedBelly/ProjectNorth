// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "PNCharacter.generated.h"

class UPNAttributeSet;
class UPNAbilitySystemComponent;

UCLASS()
class APNCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APNCharacter();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category="Gameplay Effects")
	UPNAbilitySystemComponent* AbilitySystemComponent;
	
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	
private:
	UPROPERTY()
	UPNAttributeSet* AttributeSet;
};
