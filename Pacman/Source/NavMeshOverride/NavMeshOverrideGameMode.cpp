// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NavMeshOverrideGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Coin.h"
#include "Graph.h"
#include "PlayerAIController.h"
#include "DrawDebugHelpers.h"
#include "Pacman.h"
#include "GraphGenerator.h"
#include "Array.h"
#include "BaseAIUnit.h"

#pragma optimize("", off)

ANavMeshOverrideGameMode::ANavMeshOverrideGameMode()
{
}


void ANavMeshOverrideGameMode::GenerateGraph()
{
	TArray<AActor*> ActorsToFind;
	//Find GraphGenerator
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGraphGenerator::StaticClass(), ActorsToFind);
	if (ActorsToFind.Num()) {
		GraphGenerator = Cast<AGraphGenerator>(ActorsToFind[0]);
		if (GraphGenerator)
		{
			switch (GraphGenerator->Type)
			{
			case EGraphGenerator::GGE_Default:
				GraphGenerator->GenerateGraphFromLevel();
				break;
			case EGraphGenerator::GGE_Maze:
				GraphGenerator->GenerateGraphMaze();
				break;
			default:
				break;
			}
		}
	}
}

TArray<AActor*> ANavMeshOverrideGameMode::GetTargets()
{
	TArray<AActor*> Targets;

	for (auto Coin : CoinsToCollect)
	{
		if (Coin->bIsTarget)
		{
			Targets.Add(Cast<AActor>(Coin));
		}
	}
	return Targets;
}


AActor* ANavMeshOverrideGameMode::GetCoin(AActor* OnActor)
{	
	TArray<AActor*> OverlappingCoins;
	OnActor->GetOverlappingActors(OverlappingCoins, ACoin::StaticClass());
	return OverlappingCoins.Num()? OverlappingCoins[0]: nullptr;
}

TArray<AActor*> ANavMeshOverrideGameMode::GetAllUnits(AActor* OnActor)
{
	TArray<AActor*> Res;
	Res.Add(OnActor);
	TArray<AActor*> OverlappingUnits;
	OnActor->GetOverlappingActors(OverlappingUnits, AActor::StaticClass());
	for(auto const& Actor: OverlappingUnits)
	{
		if(Cast<ABaseAIUnit>(Actor))
		{
			Res.Add(Actor);
		}
	}
	return Res;
}

void ANavMeshOverrideGameMode::BeginPlay()
{
	Super::BeginPlay();

	//Find targets for Pacman
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoin::StaticClass() , ActorsToFind);
	for (auto Coin : ActorsToFind)
	{
		CoinsToCollect.Add(Cast<ACoin>(Coin));
	}

	GenerateGraph();
}
