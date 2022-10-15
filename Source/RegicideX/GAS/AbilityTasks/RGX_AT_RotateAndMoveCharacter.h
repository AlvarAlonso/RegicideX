// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RGX_AT_RotateAndMoveCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRotateAndMoveCharacterDelegate);
class ARGX_MeleeAngel;

UCLASS()
class REGICIDEX_API URGX_AT_RotateAndMoveCharacter : public UAbilityTask
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FRotateAndMoveCharacterDelegate	OnFinish;

	URGX_AT_RotateAndMoveCharacter();

	/** Return debug string describing task */
	virtual FString GetDebugString() const override;

	void TickTask(float DeltaTime) override;
	void OnDestroy(bool AbilityIsEnding) override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static URGX_AT_RotateAndMoveCharacter* RotateAndMoveCharacter(UGameplayAbility* OwningAbility, float Time);

private:
	/* Time delay variables */
	float MaxTime = 0.0f;
	float TaskTime = 0.0f;
	ARGX_MeleeAngel* Attacker = nullptr;
	
};
