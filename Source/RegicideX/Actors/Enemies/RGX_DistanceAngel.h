// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RGX_EnemyBase.h"
#include "RGX_DistanceAngel.generated.h"

/**
 * 
 */


class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class REGICIDEX_API ARGX_DistanceAngel : public ARGX_EnemyBase
{
	GENERATED_BODY()

public:

	ARGX_DistanceAngel();


	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Ring_1_Mesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Ring_2_Mesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Ring_3_Mesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* LaserEndPointMesh;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereCollider;

	UPROPERTY(EditAnywhere)
		TArray<FVector> BombingPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotatingSpeed = 50.0f;
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FullBodyRotatingSpeed = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InterpSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxAttackDist = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinAttackDist = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HeightPos = 500.0f;

	void BeginPlay() override;

	// Called every frame
	void Tick(float DeltaTime) override;

	void RotateToTarget(float DeltaTime);

	void MoveToTarget(float DeltaTime, FVector TargetPos);

	void RotateRings(float DeltaTime);

	void RotateMe(float DeltaTime, float Speed);

	void ShootSimpleBullets();

	UFUNCTION(BlueprintCallable)
		void TestSpawn();

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnSimpleBullet(FTransform Transform, AActor* MyOwner);
};
