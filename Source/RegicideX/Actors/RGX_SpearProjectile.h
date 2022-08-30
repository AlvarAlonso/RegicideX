#pragma once

#include "CoreMinimal.h"
#include "RegicideX/Actors/Projectiles/RGX_HitboxProjectile.h"
#include "Enemies/RGX_EnemyBase.h"
#include "RGX_SpearProjectile.generated.h"

class USceneComponent;
class URGX_HitboxComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS(Blueprintable, Abstract)
class ARGX_SpearProjectile : public ARGX_HitboxProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RootScene;

public:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int RemainingSeconds = 5;

	UPROPERTY(EditDefaultsOnly)
	float TargetMaxRange = 500.0f;

	UPROPERTY()
	float Angle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARGX_EnemyBase> EnemyClass;

protected:

	UPROPERTY(EditDefaultsOnly)
	float DistanceFromCaster = 100.0f;

	UPROPERTY()
	bool bWasLaunched = false;

	UPROPERTY()
	ARGX_EnemyBase* ClosestEnemy = nullptr;

public:
	ARGX_SpearProjectile();

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	void LaunchProjectile();

	UFUNCTION(BlueprintNativeEvent)
	void HandleHit(AActor* OtherActor);

protected:

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpule, const FHitResult& Hit);

	bool IsHostile(const IGameplayTagAssetInterface* InstigatorTagInterface, const IGameplayTagAssetInterface* OtherTagInterface) const;
};