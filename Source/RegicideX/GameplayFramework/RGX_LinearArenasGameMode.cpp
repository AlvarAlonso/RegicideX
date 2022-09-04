// Fill out your copyright notice in the Description page of Project Settings.


#include "RegicideX/GameplayFramework/RGX_LinearArenasGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "RegicideX/Actors/RGX_Arena.h"

void ARGX_LinearArenasGameMode::StartPlay()
{
	Super::StartPlay();
}

void ARGX_LinearArenasGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARGX_Arena::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		ARGX_Arena* Arena = Cast<ARGX_Arena>(Actor);
		if (Arena)
		{
			Arenas.Add(Arena);
			Arena->OnArenaActivated.AddDynamic(this, &ARGX_LinearArenasGameMode::OnArenaActivated);
			Arena->OnArenaDeactivated.AddDynamic(this, &ARGX_LinearArenasGameMode::OnArenaDeactivated);
		}
	}
}

void ARGX_LinearArenasGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (ARGX_Arena* Arena : Arenas)
	{
		Arena->OnArenaActivated.RemoveDynamic(this, &ARGX_LinearArenasGameMode::OnArenaActivated);
		Arena->OnArenaDeactivated.RemoveDynamic(this, &ARGX_LinearArenasGameMode::OnArenaDeactivated);
	}
}

void ARGX_LinearArenasGameMode::OnArenaActivated(ARGX_Arena* ActivatedArena)
{
	CurrentArena = ActivatedArena;
	BP_OnArenaActivated(ActivatedArena);
}

void ARGX_LinearArenasGameMode::OnArenaDeactivated(ARGX_Arena* DeactivatedArena)
{
	CurrentArena = nullptr;
	BP_OnArenaDeactivated(DeactivatedArena);

	if(DeactivatedArena->GetIsFinalArena() == true)
	{
		BP_OnPlayerWins();
	}
}
