// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PNCharacter.h"
#include "PNPlayerCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UPNAbilitySystemComponent;

UCLASS()
class APNPlayerCharacter : public APNCharacter
{
	GENERATED_BODY()

public:
	APNPlayerCharacter();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* GameplayInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveActionAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookActionAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void HandleMoveInput(const FInputActionValue& InputActionValue);
	void HandleLookInput(const FInputActionValue& InputActionValue);

	FVector GetLookRightDir() const;
	FVector GetLookFwdDir() const;
	FVector GetMoveFwdDir() const;
};
