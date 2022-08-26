// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RegicideX/GAS/GameplayAbilities/BaseAbilities/RGX_GameplayAbility.h"
#include "RGX_GA_LaserBeamAttack.generated.h"


class ARGX_LaserBeamWeapon;



UCLASS()
class REGICIDEX_API URGX_GA_LaserBeamAttack : public URGX_GameplayAbility
{
	GENERATED_BODY()
	
public:

	URGX_GA_LaserBeamAttack();

	~URGX_GA_LaserBeamAttack();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ARGX_LaserBeamWeapon> LaserBeamWeaponSubclass;

	UPROPERTY(EditDefaultsOnly)
		float MaxTime = 0.0f;

protected:
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:

	UFUNCTION()
	void OnFinished();

};
