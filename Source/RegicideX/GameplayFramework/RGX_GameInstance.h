// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RGX_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class REGICIDEX_API URGX_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);

	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* InLoadedWorld);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loading Screen")
	TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY()
	UUserWidget* LoadingWidget;
};
