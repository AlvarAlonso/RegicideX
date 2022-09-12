
#include "RGX_PeasantController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RGX_Peasant.h"

ARGX_PeasantController::ARGX_PeasantController()
{
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BBPeasant"));
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BTPeasant"));
}

void ARGX_PeasantController::OnPossess(APawn* pawn)
{
	if (!pawn)
	{
		UE_LOG(LogTemp, Error, TEXT("No pawn to be possessed!\n"));
		return;
	}

	Super::OnPossess(pawn);
	ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(pawn);

	if (Peasant)
	{
		if (Peasant->BTree)
		{
			if (BBComponent->InitializeBlackboard(*Peasant->BTree->BlackboardAsset))
			{
				TargetActorID = BBComponent->GetKeyID("TargetActor");
				DistanceToPlayerID = BBComponent->GetKeyID("DistanceToPlayer");

				// [SM] hack pero esto hay que arreglarlo
				BBComponent->SetValueAsObject("SelfActor", GetPawn());

				// Execute behavior tree after initialization.
				BTComponent->StartTree(*Peasant->BTree, EBTExecutionMode::Looped);
			}		
		}
	}
}

void ARGX_PeasantController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const ARGX_Peasant* Peasant = Cast<ARGX_Peasant>(GetPawn());

	if (Peasant)
	{
		// Update values for the BB of the BT.
		if (Peasant->TargetActor)
		{
			BBComponent->SetValueAsObject("TargetActor", Peasant->TargetActor);
			BBComponent->SetValueAsVector("TargetLocation", Peasant->TargetActor->GetActorLocation());
		}

		// TODO Do not do it every tick
		BBComponent->SetValueAsFloat("DistanceToPlayer", Peasant->GetDistanceToTarget());
		BBComponent->SetValueAsInt("IdleAction", Peasant->IdleAction);
		BBComponent->SetValueAsBool("bFrenzied", bFrenzied);
		BBComponent->SetValueAsVector("SelfLocation", Peasant->GetActorLocation());
		BBComponent->SetValueAsVector("ForwardVector", Peasant->GetActorLocation() + Peasant->GetActorForwardVector());
	}
}
