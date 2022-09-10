
#include "RGX_DivineDescent.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RegicideX/Actors/Enemies/RGX_MeleeAngel.h"
#include "RegicideX/Actors/RGX_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

bool URGX_DivineDescent::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bResult = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if(bResult == false)
		return false;

	return true;
}

void URGX_DivineDescent::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URGX_DivineDescent::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ARGX_CharacterBase* Character = Cast<ARGX_CharacterBase>(GetAvatarActorFromActorInfo());
	if (Character)
	{
		Character->bCanRotate = true;
		Character->GetCharacterMovement()->GravityScale = 3.0f;
	}

	bTargetSelected = false;
}

void URGX_DivineDescent::Tick(float DeltaTime)
{
	if (!CurrentActorInfo)
		return;

	ARGX_MeleeAngel* owner = Cast<ARGX_MeleeAngel>(GetAvatarActorFromActorInfo());
	if (owner == nullptr || owner->bCharging == false) {
		return;
	}

	if (bFinished)
		return;

	FVector NewLocation = FVector::ZeroVector;

	//TargetLocation = owner->DivineDescentTargetLocation;
	FVector ToTarget = TargetLocation - StartLocation;
	ToTarget.Normalize();

	TargetLocation = owner->TargetActor->GetActorLocation();
	TargetLocation -= ToTarget * 150.0f; // offset to make the angel fall nearby the player and not into the player itself, causing buggy collisions
	StartLocation = owner->GetActorLocation();

	owner->RotateToTarget(DeltaTime);

	FVector MyForward = owner->GetActorForwardVector();
	MyForward.Z = ToTarget.Z;
	MyForward.Normalize();

	NewLocation = StartLocation + MyForward * Speed * DeltaTime;

	//FVector Direction = TargetLocation - StartLocation;
	//Direction.Normalize();
	//NewLocation = StartLocation + Direction * Speed * DeltaTime;

	UE_LOG(LogTemp, Warning, TEXT("NewLocation Z: %f"), NewLocation.Z);
	UE_LOG(LogTemp, Warning, TEXT("TargetLocation Z: %f"), TargetLocation.Z);

	// If we passed the z position of the target, clamp to target height and run the next section
	if (NewLocation.Z <= TargetLocation.Z)
	{
		NewLocation.Z = TargetLocation.Z;
		owner->SetActorLocation(NewLocation);
		MontageJumpToSection(FName("Fall Section"));
		UE_LOG(LogTemp, Warning, TEXT("CHAAAAAAAAAAAAARGE!"));
		owner->bCharging = false;
	}
	else
	{
		owner->SetActorLocation(NewLocation);
	}
}

TStatId URGX_DivineDescent::GetStatId() const
{
	return TStatId();
}