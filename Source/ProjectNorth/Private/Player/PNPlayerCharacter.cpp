// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PNPlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/PNAbilitySystemComponent.h"

constexpr float NegateValue = -1;

APNPlayerCharacter::APNPlayerCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 720.f, 0.f);

	bUseControllerRotationYaw = false;
}

void APNPlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (APlayerController* OwningPlayerController = GetController<APlayerController>())
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			OwningPlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (InputSubsystem)
		{
			InputSubsystem->RemoveMappingContext(GameplayInputMappingContext);
			InputSubsystem->AddMappingContext(GameplayInputMappingContext, 0);
		}
	}
}

void APNPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveActionAction, ETriggerEvent::Triggered, this,&APNPlayerCharacter::HandleMoveInput);
		EnhancedInputComponent->BindAction(LookActionAction, ETriggerEvent::Triggered, this,&APNPlayerCharacter::HandleLookInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APNCharacter::Jump);

		for (auto KV : GameplayAbilityInputActions)
		{
			EnhancedInputComponent->BindAction(KV.Value, ETriggerEvent::Triggered, this, &APNPlayerCharacter::HandleAbilityInput, KV.Key);
		}
	}
}

void APNPlayerCharacter::HandleMoveInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	InputValue.Normalize();

	AddMovementInput(GetMoveFwdDir() * InputValue.Y + GetLookRightDir() * InputValue.X);
}

void APNPlayerCharacter::HandleLookInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	AddControllerPitchInput(InputValue.Y * NegateValue);
	AddControllerYawInput(InputValue.X);
}

void APNPlayerCharacter::HandleAbilityInput(const FInputActionValue& InputActionValue, EAbilityInputID InputID)
{
	if (InputActionValue.Get<bool>())
	{
		GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<int32>(InputID));
	}
	else
	{
		GetAbilitySystemComponent()->AbilityLocalInputReleased(static_cast<int32>(InputID));
	}
}

FVector APNPlayerCharacter::GetLookRightDir() const
{
	return Camera->GetRightVector();
}

FVector APNPlayerCharacter::GetLookFwdDir() const
{
	return Camera->GetForwardVector();
}

FVector APNPlayerCharacter::GetMoveFwdDir() const
{
	return FVector::CrossProduct(GetLookRightDir(), FVector::UpVector);
}
