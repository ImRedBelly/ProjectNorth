// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "PNCharacter.generated.h"

class UPNAttributeSet;
class UPNAbilitySystemComponent;

UCLASS()
class APNCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APNCharacter();
	void ServerSideInit();
	void ClientSideInit();
	bool IsLocallyControllerByPlayer() const;
	
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void OnDeath();
	virtual void OnRespawn();

private:
	UPROPERTY(VisibleDefaultsOnly, Category="Gameplay Ability")
	UPNAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UPNAttributeSet* AttributeSet;

	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	class UWidgetComponent* OverHeadWidgetComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	float HeadStatGaugeVisibilityCheckUpdateGap = 1.f;

	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	float HeadStatGaugeVisibilityRangeSquared = 10000000.f;

	UPROPERTY(EditAnywhere, Category = "Death")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, Category = "Death")
	float DeathMontageFinishTimeShift = -0.8f;

	FTransform MeshRelativeTransform;
	
	FTimerHandle HeadStatGaugeVisibilityUpdateTimerHandle;
	FTimerHandle DeathTimerHandle;
	
	UPROPERTY(Replicated)
	FGenericTeamId TeamID;

	void UpdateHeadStatGaugeVisibility() const;
	void SetStatusGaugeEnable(bool bIsEnable);
	void ConfigureOverHeadStatusWidget();
	void BindGASChangeDelegated();
	void DeathTagUpdated(const FGameplayTag Tag, int NewCount);

	void StartDeathSequence();
	void Respawn();

	void DeathFinished();
	void SetRagdollEnable(bool bIsEnable);
};
