// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AN_SendTargetGroup.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTagsManager.h"

void UAN_SendTargetGroup::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;
	if (!MeshComp->GetOwner()) return;
	if (TargetSocketNames.Num() <= 1) return;
	if (!UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner())) return;

	FGameplayEventData EventData;
	for (int i = 1; i < TargetSocketNames.Num(); ++i)
	{
		FGameplayAbilityTargetData_LocationInfo* LocationInfo = new FGameplayAbilityTargetData_LocationInfo();

		FVector StartLocation = MeshComp->GetSocketLocation(TargetSocketNames[i - 1]);
		FVector EndLocation = MeshComp->GetSocketLocation(TargetSocketNames[i]);
		LocationInfo->SourceLocation.LiteralTransform.SetLocation(StartLocation);
		LocationInfo->TargetLocation.LiteralTransform.SetLocation(EndLocation);

		EventData.TargetData.Add(LocationInfo);
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, EventData);
}

FString UAN_SendTargetGroup::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		TArray<FName> TagNames;
		UGameplayTagsManager::Get().SplitGameplayTagFName(EventTag, TagNames);
		return TagNames.Last().ToString();
	}
	return "None";
}
