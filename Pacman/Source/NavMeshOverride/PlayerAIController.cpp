// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "AI/Navigation/NavigationSystem.h"
#include "LogMacros.h"
#include "NavMeshOverrideGameMode.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/NumericLimits.h"
#include "Graph.h"

#pragma optimize("", off)


void APlayerAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (Targets.Num())
	//{
	//	SetNewMoveDestination(Targets[0]->GetActorLocation());

	//}

}

void APlayerAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
}

void APlayerAIController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();

		if (NavSys)
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void APlayerAIController::RunBFS()
{
	ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM || !GM->CoinsToCollect.Num())
	{
		return;
	}

	UGraph* G = GM->GenerateGraphFromLevel();

	if (GetPawn() && GM->GetTarget())
	{
		Targets = G->BFS(GetPawn(), GM->GetTarget());
	}

}

void APlayerAIController::RunDFS()
{
	ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM || !GM->CoinsToCollect.Num())
	{
		return;
	}


	UGraph* G = GM->GenerateGraphFromLevel();

	if (GetPawn() && GM->GetTarget())
	{
		Targets = G->DFS(GetPawn(), GM->GetTarget());
	}
}

void APlayerAIController::BeginPlay()
{
	Super::BeginPlay();
}
