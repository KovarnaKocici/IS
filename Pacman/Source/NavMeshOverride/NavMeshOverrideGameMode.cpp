// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NavMeshOverrideGameMode.h"
#include "NavMeshOverridePlayerController.h"
#include "NavMeshOverrideCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Coin.h"
#include "Graph.h"
#include "PlayerAIController.h"
#include "DrawDebugHelpers.h"
#include "Pacman.h"
#include "Array.h"

#pragma optimize("", off)

ANavMeshOverrideGameMode::ANavMeshOverrideGameMode()
{
}


void ANavMeshOverrideGameMode::GenerateGraph(bool bIncludeOnlyTargets)
{
	//Find targets for Pacman
	TArray<AActor*> ActorsToFind;
	//Create Graph from found targets
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGraph::StaticClass(), ActorsToFind);
	Graph = Cast<AGraph>(ActorsToFind[0]);
	if (Graph)
	{
		Graph->GenerateGraphFromLevel(bIncludeOnlyTargets);
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
}
