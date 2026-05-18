// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/PNGameplayAbility.h"

#include "Kismet/KismetSystemLibrary.h"

UAnimInstance* UPNGameplayAbility::GetOwnerAnimInstance() const
{
	if (const USkeletalMeshComponent* OwnerSkeletalMeshComponent = GetOwningComponentFromActorInfo())
	{
		return OwnerSkeletalMeshComponent->GetAnimInstance();
	}
	return nullptr;
}

TArray<FHitResult> UPNGameplayAbility::GetHitResultsFromSweepLocationTargetData(
	const FGameplayAbilityTargetDataHandle& TargetDataHandle,
	float SphereSweepRadius,
	ETeamAttitude::Type TargetTeam,
	bool bIgnoreSelf,
	bool bDrawDebug) const
{
	TArray<FHitResult> OutResult;
	TArray<AActor*> HitActors;

	IGenericTeamAgentInterface* OwnerTeamInterface = Cast<IGenericTeamAgentInterface>(GetAvatarActorFromActorInfo());

	for (auto TargetData : TargetDataHandle.Data)
	{
		FVector StartLocation = TargetData->GetOrigin().GetTranslation();
		FVector EndLocation = TargetData->GetEndPoint();

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

		TArray<AActor*> IgnoreActors;
		if (bIgnoreSelf) IgnoreActors.Add(GetAvatarActorFromActorInfo());

		EDrawDebugTrace::Type DrawDebugTrace = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

		TArray<FHitResult> Results;
		UKismetSystemLibrary::SphereTraceMultiForObjects(this,
		                                                 StartLocation,
		                                                 EndLocation,
		                                                 SphereSweepRadius,
		                                                 ObjectTypes,
		                                                 false,
		                                                 IgnoreActors,
		                                                 DrawDebugTrace,
		                                                 Results,
		                                                 false);

		for (const FHitResult& Result : Results)
		{
			if (HitActors.Contains(Result.GetActor())) continue;

			if (OwnerTeamInterface)
			{
				ETeamAttitude::Type OtherTeamAttitude = OwnerTeamInterface->GetTeamAttitudeTowards(*Result.GetActor());
				if (OtherTeamAttitude != TargetTeam)
				{
					continue;
				}
			}

			HitActors.Add(Result.GetActor());
			OutResult.Add(Result);
		}
	}

	return OutResult;
}
