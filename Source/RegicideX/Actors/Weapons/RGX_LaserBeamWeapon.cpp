// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_LaserBeamWeapon.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARGX_LaserBeamWeapon::ARGX_LaserBeamWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PathSplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("PathSplineComponent"));
	EndPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndPointMesh"));
	EndPointParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EndPointParticle"));
}

// Called when the game starts or when spawned
void ARGX_LaserBeamWeapon::BeginPlay()
{
	Super::BeginPlay();
	//TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
}


void ARGX_LaserBeamWeapon::ComputeNewEndpoint(float DeltaTime)
{
	const FVector MyLocation = EndPointMesh->K2_GetComponentLocation();

	if (FollowTarget)
	{
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const FRotator RotOffset = UKismetMathLibrary::FindLookAtRotation(MyLocation, TargetLocation);
		EndPointMesh->SetWorldRotation(RotOffset);
	}

	const FVector MyForward = EndPointMesh->GetForwardVector();

	FVector NewLocation = MyLocation + MyForward * RaySpeed * DeltaTime; NewLocation.Z = 100;

	FHitResult Result;

	FVector RaySrc = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 200.0;
	FVector RayEndPoint = NewLocation + MyForward;

	UKismetSystemLibrary::DrawDebugLine(
		GetWorld(),
		RaySrc,
		RayEndPoint,
		FColor(255, 0, 0),
		DeltaTime,
		5.0f
	);
	UKismetSystemLibrary::DrawDebugPoint(GetWorld(), RaySrc, 22, FColor(255, 0, 255), DeltaTime);

	if(GetWorld()->LineTraceSingleByChannel(Result, RaySrc, NewLocation, ECollisionChannel::ECC_WorldStatic))
	{
		FVector ImpactPoint = Result.ImpactPoint;
		NewLocation = ImpactPoint;
	}

	EndPointMesh->SetWorldLocation(NewLocation);
}


void ARGX_LaserBeamWeapon::MoveRay(float DeltaTime)
{
	ComputeNewEndpoint(DeltaTime);

	const FVector EndPoint = EndPointMesh->K2_GetComponentLocation();

	PathSplineComponent->ClearSplinePoints(true);

	PathSplineComponent->AddSplinePointAtIndex(SourcePoint, 0, ESplineCoordinateSpace::World);
	PathSplineComponent->AddSplinePointAtIndex(EndPoint, 1, ESplineCoordinateSpace::World);

	PathSplineComponent->SetSplinePointType(1, ESplinePointType::CurveClamped);

	if(!PathSplineMeshes.IsValidIndex(0))
	{
		SpawnSplineMesh();
		PathSplineMeshes[0]->SetStartScale(FVector2D(2.f));
		PathSplineMeshes[0]->SetEndScale(FVector2D(2.f));
		//PathSplineMeshes[0]->SetMaterial(0, Material);
	}

	FVector StartTangent = PathSplineComponent->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::World);
	FVector EndTangent = PathSplineComponent->GetTangentAtSplinePoint(1, ESplineCoordinateSpace::World);

	PathSplineMeshes[0]->SetStartAndEnd(SourcePoint, StartTangent, EndPoint, EndTangent);
}

void ARGX_LaserBeamWeapon::SetSourcePoint(FVector SP)
{
	SourcePoint = SP;
	EndPointMesh->SetWorldLocation(SP);
}

void ARGX_LaserBeamWeapon::SetOwnerActor(AActor* OA)
{
	OwnerActor = OA;
}



