#include "RGX_CombatAssistComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Actors/Enemies/RGX_EnemyBase.h"

URGX_CombatAssistComponent::URGX_CombatAssistComponent()
{

}

void URGX_CombatAssistComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URGX_CombatAssistComponent::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void URGX_CombatAssistComponent::PerformAttackAutoAssist()
{
	AActor* PlayerActor = GetOwner();

	const FVector PlayerLocation = PlayerActor->GetActorLocation();

	const float radius = 300.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UClass* SeekClass = ARGX_EnemyBase::StaticClass();

	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;

	// Check for nearby enemies
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), PlayerLocation, radius, TraceObjectTypes, SeekClass, IgnoreActors, OutActors) == false)
		return;

	float CurrentClosestDistance = INFINITY;
	FVector NearestEnemyLocation = FVector(0.0f, 0.0f, 0.0f);
	bool bHasTarget = false;

	// Check the closest enemy inside a cone in front of the player
	for (AActor* Actor : OutActors)
	{
		ARGX_EnemyBase* Enemy = Cast<ARGX_EnemyBase>(Actor);

		const bool bIsDead = Enemy->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Dead")));

		if (bIsDead == true)
			continue;

		const FVector EnemyLocation = Enemy->GetActorLocation();

		const float Distance = FVector::Dist(PlayerLocation, EnemyLocation);

		FVector PlayerToEnemyVector = EnemyLocation - PlayerLocation;
		PlayerToEnemyVector.Normalize();

		const FVector PlayerForward = PlayerActor->GetActorForwardVector();

		// Cone check
		const float Dot = FVector::DotProduct(PlayerToEnemyVector, PlayerForward);

		if (Distance < CurrentClosestDistance && Dot > 0.5f)
		{
			CurrentClosestDistance = Distance;
			NearestEnemyLocation = EnemyLocation;
			bHasTarget = true;
		}
	}

	if (bHasTarget == false)
		return;

	const FVector PlayerToEnemyVector = NearestEnemyLocation - PlayerLocation;
	const FRotator Rotation = UKismetMathLibrary::MakeRotFromX(PlayerToEnemyVector);

	PlayerActor->SetActorRotation(Rotation);

	const float OffsetToEnemy = 150.0f;

	if (CurrentClosestDistance > OffsetToEnemy == false)
		return;

	FVector AssistDirection = FVector(PlayerToEnemyVector.X, PlayerToEnemyVector.Y, 0.0f);
	AssistDirection.Normalize();

	const FVector FinalLocation = PlayerLocation + AssistDirection * (CurrentClosestDistance - OffsetToEnemy);

	PlayerActor->SetActorLocation(FinalLocation);
}
