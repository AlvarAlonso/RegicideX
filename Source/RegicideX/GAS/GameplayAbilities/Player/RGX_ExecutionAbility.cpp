// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GAS/GameplayAbilities/Player/RGX_ExecutionAbility.h"
#include "RegicideX/Actors/Enemies/RGX_EnemyBase.h"
#include "RegicideX/Character/RGX_PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemGlobals.h"

URGX_ExecutionAbility::URGX_ExecutionAbility()
{

}

bool URGX_ExecutionAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if (bResult == false)
		return false;

	ARGX_PlayerCharacter* PlayerCharacter = Cast<ARGX_PlayerCharacter>(ActorInfo->AvatarActor);
	if (PlayerCharacter == nullptr)
		return false;

	return true;
}

void URGX_ExecutionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	const ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(TriggerEventData->Target);
	
	if (Enemy == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	TargetActor = Enemy;

	UE_LOG(LogTemp, Warning, TEXT("Execution Ability\n"));

	const FVector PlayerLocation = ActorInfo->AvatarActor->GetActorLocation();
	const FVector EnemyLocation = Enemy->GetActorLocation();

	FVector PlayerToEnemy = EnemyLocation - PlayerLocation;
	PlayerToEnemy.Z = 0.0f;
	PlayerToEnemy.Normalize();

	FRotator PlayerTargetRotation = UKismetMathLibrary::MakeRotFromX(PlayerToEnemy);
	PlayerTargetRotation.Pitch = 0.0f;
	PlayerTargetRotation.Roll = 0.0f;

	ActorInfo->AvatarActor->SetActorRotation(PlayerTargetRotation);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_ExecutionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URGX_ExecutionAbility::OnReceivedEvent(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("Tag: %s\n"), *EventTag.ToString());
	UAbilitySystemComponent* ACS = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	if (ACS)
	{
		FGameplayEventData EventData;
		EventData.Instigator = CurrentActorInfo->AvatarActor.Get();
		EventData.Target = TargetActor;
		ACS->HandleGameplayEvent(EventTag, &EventData);
	}
}
