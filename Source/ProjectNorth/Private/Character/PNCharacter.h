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
	void ServerSideInit();
	void ClientSideInit();
	bool IsLocallyControllerByPlayer() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category="Gameplay Ability")
	UPNAbilitySystemComponent* AbilitySystemComponent;
	UPROPERTY()
	UPNAttributeSet* AttributeSet;

	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	class UWidgetComponent* OverHeadWidgetComponent;

	void ConfigureOverHeadStatusWidget();


	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	float HeadStatGaugeVisibilityCheckUpdateGap = 1.f;
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	float HeadStatGaugeVisibilityRangeSquared = 10000000.f;

	FTimerHandle HeadStatGaugeVisibilityUpdateTimerHandle;
	void UpdateHeadStatGaugeVisibility() const;
};
