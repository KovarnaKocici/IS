// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAIController.h"
#include "NavMeshOverride.h"
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
#include "UObjectGlobals.h"

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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGraph::StaticClass(), Graphs);

	if (Graphs.Num())
	{
		double StartTime = 0.f;
		double EndTime = 0.f;

		AGraph* G = Cast<AGraph>(Graphs[0]);

		if (GetPawn() && GM->GetTargets().Num() && G)
		{
			StartTime = FPlatformTime::Seconds();
			switch (Method)
			{
			case EPathfinderEnum::PE_BFS:
				Targets = G->BFS(GM->GetCoin(GetPawn()), GM->GetTargets()[0]);
				break;
			case EPathfinderEnum::PE_DFS:
				Targets = G->DFS(GM->GetCoin(GetPawn()), GM->GetTargets()[0]);
				break;
			case EPathfinderEnum::PE_Dijkstra:
				Targets = G->Dijkstra(GM->GetCoin(GetPawn()), GM->GetTargets()[0]);
				break;
			case EPathfinderEnum::PE_AStar:
				Targets = G->AStar(GM->GetCoin(GetPawn()), GM->GetTargets()[0]);
				break;
			default:
				break;
			}
			EndTime = FPlatformTime::Seconds();

			float PathPrice = 0.f;
			for (auto Target : Targets)
			{
				PathPrice += Cast<ACoin>(Target)->Price;
			}

			UE_LOG(LogTemp, Warning, TEXT("\n Was run %s\nTime elapsed %f\nNumber of Graph nodes %i\nLength of Path %i\nPrice of Path %f\n"),
				*GETENUMSTRING("EPathfinderEnum", Method), EndTime - StartTime, G->GetSize(), Targets.Num(), PathPrice);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PATHFINDER CANT BE RUN"));
		}
	}

	//G->Destroy();
	//G = nullptr;
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
