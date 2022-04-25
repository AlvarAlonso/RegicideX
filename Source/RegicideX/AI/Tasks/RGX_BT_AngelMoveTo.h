// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RGX_BT_AngelMoveTo.generated.h"

/**
 * 
 */


UCLASS()
class REGICIDEX_API URGX_BT_AngelMoveTo : public UBTTaskNode
{
	GENERATED_BODY()
	
	EBTNodeResult::Type TaskResult;

public:

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};


















