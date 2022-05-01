#pragma once

#include "CoreMinimal.h"
#include "Abilities/MCV_GameplayAbility.h"
#include "RGX_GA_DashAbility.generated.h"

class ACharacter;

UCLASS()
class REGICIDEX_API URGX_DashAbility : public UMCV_GameplayAbility
{
	GENERATED_BODY()

	//virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	float DashForce;

	UPROPERTY(EditDefaultsOnly)
	float DashDuration;

protected:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetDashDuration();

	UFUNCTION(BlueprintCallable)
	void PerformDash();
};