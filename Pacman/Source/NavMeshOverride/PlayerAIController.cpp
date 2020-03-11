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

void APlayerAIController::RunPathfinder(EPathfinderEnum Method)
{
	ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM || !GM->CoinsToCollect.Num())
	{
		return;
	}

	TArray<AActor*> Graphs;
	TSubclassOf<AGraph> classToFind;
	classToFind = AGraph::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, Graphs);

	if (Graphs.Num())
	{
		AGraph* G = Cast<AGraph>(Graphs[0]);

		if (GetPawn() && GM->GetTargets().Num())
		{
			switch (Method)
			{
			case EPathfinderEnum::PE_BFS:
				Targets = G->BFS(GetPawn(), GM->GetTargets()[0]);
				break;
			case EPathfinderEnum::PE_DFS:
				Targets = G->DFS(GetPawn(), GM->GetTargets()[0]);
				break;
			case EPathfinderEnum::PE_Dijkstra:
				Targets = G->Dijkstra(GetPawn(), GM->GetTargets()[0]);
				break;
			case EPathfinderEnum::PE_AStar:
				Targets = G->Dijkstra(GetPawn(), GM->GetTargets()[0]);
				break;
			default:
				break;
			}
		}
	}
}

void APlayerAIController::RunDijkstra()
{
	RunPathfinder(EPathfinderEnum::PE_Dijkstra);
}

void APlayerAIController::RunBFS()
{
	RunPathfinder(EPathfinderEnum::PE_BFS);
}

void APlayerAIController::RunDFS()
{
	RunPathfinder(EPathfinderEnum::PE_DFS);
}

void APlayerAIController::BeginPlay()
{
	Super::BeginPlay();
}
