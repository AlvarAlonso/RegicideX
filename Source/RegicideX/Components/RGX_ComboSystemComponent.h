#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags.h"
#include "../Enums/RGX_InputEnums.h"
#include "RGX_ComboSystemComponent.generated.h"

USTRUCT()
struct FRGX_ComboTransition
{
	GENERATED_BODY()

	// Possible inputs to continue with the combo
	UPROPERTY(EditAnywhere)
	TArray<ERGXPlayerInputID> NextPotentialInputs;

	// Possible attack transitions
	UPROPERTY(EditAnywhere)
	TArray<FGameplayTag> NextPotentialAttacks;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class REGICIDEX_API URGX_ComboSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URGX_ComboSystemComponent();

	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	FGameplayTag ManageInputToken(ERGXPlayerInputID PlayerInput);
	FGameplayTag GetNextAttack();
	void CleanStatus(int32 ActivatedAbilities);

protected:

	UPROPERTY(EditDefaultsOnly, Category = Combo)
	TMap<FGameplayTag, FRGX_ComboTransition> ComboMap;

protected:
	// Combo managing variables
	UPROPERTY()
	FGameplayTag CurrentAttack = FGameplayTag::RequestGameplayTag("Combo.None");

	UPROPERTY()
	FGameplayTag NextAttack = FGameplayTag::RequestGameplayTag("Combo.None");

	UPROPERTY()
	ERGXPlayerInputID NextComboInput;

	UPROPERTY()
	bool bEnableComboFlag = false;

	UPROPERTY()
	bool bComboFlag = false;
	// ------------------------

protected:

	UFUNCTION()
	void SetNextComboAttack(ERGXPlayerInputID PlayerInput);

	UFUNCTION()
	void InitiateCombo(ERGXPlayerInputID PlayerInput);

	UFUNCTION()
	FGameplayTag FindNextAttack(ERGXPlayerInputID PlayerInput);

public:

	UFUNCTION(BlueprintCallable, Category = "Combo", Meta = (DisplayName = "IsAttacking"))
	bool IsAttacking();

	UFUNCTION(BlueprintCallable, Category = "Combo", Meta = (DisplayName = "OnCombo"))
	void OnCombo();

	UFUNCTION(BlueprintCallable, Category = "Combo", Meta = (DisplayName = "OnEnableCombo"))
	void OnEnableCombo();

	UFUNCTION(BlueprintCallable, Category = "Combo", Meta = (DisplayName = "OnDisableCombo"))
	void OnDisableCombo();

	UFUNCTION(BlueprintCallable, Category = "Combo", Meta = (DisplayName = "OnEndCombo"))
	void OnEndCombo();

	UFUNCTION()
	void DrawDebugInfo();
};