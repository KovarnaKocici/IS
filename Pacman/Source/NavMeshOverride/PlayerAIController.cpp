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
#include "BaseAIUnit.h"
#include "UObjectGlobals.h"
#include "Pacman.h"
#include "Ghost.h"

#pragma optimize("", off)


void APlayerAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsReadyToRun)
	{ 
		if(!bShouldEvaluateOnce && bEvaluate)
		{ 
			RunPathfinder(CurrentPathfinder);
			bEvaluate = false;
			++EvaluationTimes;
		}
		else if (EvaluationTimes < 1)
		{
			RunPathfinder(CurrentPathfinder);
			++EvaluationTimes;
		}
	}
}

void APlayerAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
}

void APlayerAIController::RunPathfinder(EPathfinder Method)
{
	if (AGraph* G = GetCurrentGraph())
	{
		TArray<TArray<AActor*>> Paths;
		ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());

		double StartTime = 0.f;
		double EndTime = 0.f;

		if (GetPawn() && GM->GetTargets().Num() && G)
		{
			StartTime = FPlatformTime::Seconds();
			switch (Method)
			{
			case EPathfinder::PE_BFS:
				Targets = G->BFS(GM->GetCoin(GetPawn()), GM->GetTargets()[0]);
				break;
			case EPathfinder::PE_DFS:
				Targets = G->DFS(GM->GetCoin(GetPawn()), GM->GetTargets()[0]);
				break;
			case EPathfinder::PE_Dijkstra:
				Targets = G->Dijkstra(GM->GetCoin(GetPawn()), GM->GetTargets()[0]);
				break;
			case EPathfinder::PE_AStar:
				Targets = G->AStar(GM->GetCoin(GetPawn()), GM->GetTargets()[0]);
				break;
			case EPathfinder::PE_MinMax:
				//Find all paths of length K
				Paths = G->GetAllPathsOfLength(GM->GetCoin(GetPawn()), SearchDepth);
				//Choose one with the best sum result
				Targets = GetBestPath(Paths);
				break;
			default:
				break;
			}
			EndTime = FPlatformTime::Seconds();
			PrintPathfinderSummary(Method, StartTime, EndTime);
			bNewPathWasFound = true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PATHFINDER CANT BE RUN"));
		}
	}

	//G->Destroy();
	//G = nullptr;
}

float APlayerAIController::EvaluatePath(TArray<AActor*> Path)
{
	float Evaluation = 0.0f;

	if (AGraph* G = GetCurrentGraph())
	{ 
		ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());
		for(auto const &Node: Path)
		{
			TArray<AActor*> UnitsOnNode = GM->GetAllUnits(Node);
			for (auto const &Unit : UnitsOnNode)
			{
				if (Cast<APacman>(Unit))
				{
					Evaluation += Params.Pacman;
				}
				if (Cast<AGhost>(Unit))
				{
					Evaluation += Params.Ghost;
				}
				if (ACoin* Coin = Cast<ACoin>(Unit))
				{
					if (Coin->bIsActive)
					{
						Evaluation += Params.TragetCoin;
					}
					else
					{
						Evaluation += Params.BaseCoin;
					}
				}
			}
		}
	}

	return Evaluation;
}

TArray<AActor*> APlayerAIController::GetBestPath(TArray<TArray<AActor*>> Paths)
{
	TArray<AActor*> BestPath;
	if(ABaseAIUnit* CurrentPawn = Cast<ABaseAIUnit>(GetPawn()))
	{
		float BestEvaluation = -1000, CurrentEvaluation = -1000;

		for (auto &Path : Paths)
		{
			CurrentEvaluation = EvaluatePath(Path);
			if (CurrentEvaluation > BestEvaluation)
			{
				BestPath = Path;
			}
		}
	}
	return BestPath;
}

AGraph* APlayerAIController::GetCurrentGraph()
{
	ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM || !GM->CoinsToCollect.Num())
	{
		return nullptr;
	}

	TArray<AActor*> Graphs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGraph::StaticClass(), Graphs);

	return Cast<AGraph>(Graphs[0]);
}

void APlayerAIController::PrintPathfinderSummary(EPathfinder Method, double StartTime, double EndTime)
{
	float PathPrice = 0.f;
	for (auto Target : Targets)
	{
		PathPrice += Cast<ACoin>(Target)->Price;
	}
	UE_LOG(LogTemp, Warning, TEXT("\nUnit %s was executing %s\nTime elapsed %f\nNumber of Graph nodes %i\nLength of Path %i\nPrice of Path %f\n"),
		*GetPawn()->GetName(), *GETENUMSTRING("EPathfinder", Method), EndTime - StartTime, GetCurrentGraph()->GetSize(), Targets.Num(), PathPrice);

}

void APlayerAIController::BeginPlay()
{
	Super::BeginPlay();
}
