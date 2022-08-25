// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "RGX_EnemyBase.h"
#include "RGX_Peasant.generated.h"

class URGX_MovementAssistComponent;
class UBehaviorTree;
class ARGX_GroupManager;

UCLASS()
class REGICIDEX_API ARGX_Peasant : public ARGX_EnemyBase
{
	GENERATED_BODY()
public:

	ARGX_Peasant();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URGX_MovementAssistComponent* MovementAssistComponent;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BTree = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float WanderSpeed = 100.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int IdleAction;

	float GetDistanceToTarget() const;

protected:

	UPROPERTY()
	FVector WanderingPoint;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	// Bool to signal if actor is going to get destroyed.
	void HandleDeath() override;
};
