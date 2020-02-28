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

#pragma optimize("", off)

ANavMeshOverrideGameMode::ANavMeshOverrideGameMode()
{
}


UGraph* ANavMeshOverrideGameMode::GenerateGraphFromLevel()
{
	if (CoinsToCollect.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Was found %i actors"), CoinsToCollect.Num());
	}

	//Initialize Graph
	TArray<AActor*> Edges;

	//Random Relations are generated with certain probability
	UGraph* G = NewObject<UGraph>();
	
	//Fins all targets
	//Find PlayerAI and add its pawn
	TArray<AActor*> Players;
	TSubclassOf<APacman> classToFind;
	classToFind  = APacman::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, Players);
	if (Players.Num())
	{
		Edges.Add(Cast<AActor>(Players[0]));
	}

	for (auto Coin : CoinsToCollect)
	{
		Edges.Add(Cast<AActor>(Coin));
		G->AddNode(Edges.Last(0));
	}

	for (auto ActorL : Edges)
	{
		for (auto ActorR : Edges)
		{
			if (FMath::RandRange(0.f, 1.f) < Density)
			{
				G->AddRelation(ActorL, ActorR);
				DrawDebugLine(GetWorld(), ActorL->GetActorLocation(), ActorR->GetActorLocation(), FColor::White, true, 0.f);
			}
		}
	}

	return G;
}

AActor* ANavMeshOverrideGameMode::GetTarget()
{
	AActor* Target = nullptr;
	for (auto Coin : CoinsToCollect)
	{
		if (Coin->bIsTarget)
		{
			Target = Cast<AActor>(Coin);
			break;
		}
	}
	return Target;
}

void ANavMeshOverrideGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Coins;
	TSubclassOf<ACoin> classToFind;
	classToFind = ACoin::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, Coins);
	for (auto Coin : Coins)
	{
		CoinsToCollect.Add(Cast<ACoin>(Coin));
	}
}
