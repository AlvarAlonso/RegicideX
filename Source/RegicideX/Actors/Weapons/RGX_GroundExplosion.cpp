// Fill out your copyright notice in the Description page of Project Settings.


#include "RGX_GroundExplosion.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameplayTags.h"

// Sets default values
ARGX_GroundExplosion::ARGX_GroundExplosion()
	: AActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ExplosionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollider"));
	ExplosionCollider->SetupAttachment(RootComponent);

	CircumferenceDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CircumferenceDecal"));
	CircumferenceDecal->SetupAttachment(RootComponent);

	AreaDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("AreaDecal"));
	AreaDecal->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARGX_GroundExplosion::BeginPlay()
{
	Super::BeginPlay();

	CircumRef = CircumferenceDecal->CreateDynamicMaterialInstance();
	CircumRef->SetScalarParameterValue(FName("Circum1Area0"), 1.0);

	AreaRef = AreaDecal->CreateDynamicMaterialInstance();
	AreaRef->SetScalarParameterValue(FName("Circum1Area0"), 0.0);

	if (AreaCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;
		TimelineFinishedCallback.BindLambda([this]() { Explode(); });

		AreaTimeLine.AddInterpFloat(AreaCurve, TimelineCallback);
		AreaTimeLine.SetTimelineFinishedFunc(TimelineFinishedCallback);

		AreaTimeLine.PlayFromStart();
	}
	//GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandle)
}

void ARGX_GroundExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AreaCurve)
	{
		AreaTimeLine.TickTimeline(DeltaTime);

		AreaRef->SetScalarParameterValue(FName("AreaValue"), AreaCurve->GetFloatValue(AreaTimeLine.GetPlaybackPosition()));
	}
}

void ARGX_GroundExplosion::Explode()
{
	TArray<AActor*> OverlappedActors;
	ExplosionCollider->GetOverlappingActors(OverlappedActors, TargetActor);

	for (AActor* Actor : OverlappedActors)
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, true))
		{
			if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("PossessedBy.Player")))
			{
				ASC->ApplyGameplayEffectToSelf(ExplosionEffect->GetDefaultObject<UGameplayEffect>(), 1.0, ASC->MakeEffectContext());
			}
		}
	}

	// Spawn Emitter

	Destroy();
}
