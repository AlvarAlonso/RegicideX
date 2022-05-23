#include "RGX_GE_Death.h"

URGX_DeathEffect::URGX_DeathEffect()
{
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));
	DurationPolicy = EGameplayEffectDurationType::Infinite;
}

URGX_HitEffect::URGX_HitEffect()
{
	InheritableOwnedTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("GameplayEvent.Combat.TakeDamage")));
	DurationPolicy = EGameplayEffectDurationType::Infinite;
}