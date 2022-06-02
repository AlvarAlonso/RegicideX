#include "RGX_HealthAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "RegicideX/GAS/GameplayEffects/RGX_GE_Death.h"
#include "RegicideX/GAS/GameplayEffects/RGX_GE_HitEffect.h"

void URGX_HealthAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
}

void URGX_HealthAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	UE_LOG(LogTemp, Warning, TEXT("New Health: %f\n"), NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
}

void URGX_HealthAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		bool bIsDead = ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));
		if (bIsDead == false)
		{
			float CurrentHealth = GetHealth();
			if (CurrentHealth <= 0)
			{
				// If current health is still 0, we die
				CurrentHealth = GetHealth();
				if (CurrentHealth <= 0)
				{
					UGameplayEffect* DeathEffect = URGX_DeathEffect::StaticClass()->GetDefaultObject<UGameplayEffect>();
					ASC->ApplyGameplayEffectToSelf(DeathEffect, 1, ASC->MakeEffectContext());

					FGameplayEventData EventData;
					ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.HasDied")), &EventData);
				}
			}
			else
			{
				UGameplayEffect* HitEffect = URGX_HitEffect::StaticClass()->GetDefaultObject<UGameplayEffect>();
				ASC->ApplyGameplayEffectToSelf(HitEffect, 1, ASC->MakeEffectContext());

				{
					FGameplayEventData EventData;
					EventData.Instigator = Data.EffectSpec.GetEffectContext().GetInstigator();
					ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Combat.TakeDamage")), &EventData);
				}
			

				if (CurrentHealth < GetMaxHealth() * 0.25f)
				{
					FGameplayEventData EventData;
					ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Enemy.Weakened")), &EventData);
				}
			}
		}
	}
}