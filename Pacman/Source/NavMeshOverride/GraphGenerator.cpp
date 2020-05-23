// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphGenerator.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "NavMeshOverrideGameMode.h"
#include "Coin.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGraphGenerator::AGraphGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}


bool AGraphGenerator::CanGenerateGraph()
{
	ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM || !GM->CoinsToCollect.Num())
	{
		return false;
	}

	UE_LOG(LogTemp, Error, TEXT("Was found %i nodes for graph initialization"), GM->CoinsToCollect.Num());
	return true;

}

void AGraphGenerator::GenerateGraphFromLevel()
{
	if (Graph && CanGenerateGraph())
	{
		//Graph->Clear();
		ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());

		//Initialize Graph
		TArray<AActor*> Vertexes;

		for (auto Coin : GM->CoinsToCollect)
		{
			Vertexes.Add(Cast<AActor>(Coin));
		}


		TArray<AActor*> NearestCoins;

		for (auto ActorL : Vertexes)
		{
			ActorL->GetOverlappingActors(NearestCoins, ACoin::StaticClass());

			for (auto ActorR : NearestCoins)
			{
				if (ActorL != ActorR)
				{
					Graph->AddRelation(ActorL, ActorR);
					Graph->AddRelation(ActorR, ActorL);
					DrawDebugLine(GetWorld(), ActorL->GetActorLocation(), ActorR->GetActorLocation(), FColor::White, true, 0.f);
				}
			}
		}
	}
}

void AGraphGenerator::GenerateGraphMaze()
{
	if (Graph && CanGenerateGraph())
	{
		//Graph->Clear();
		ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());

		//Initialize Graph
		for (auto Coin : GM->CoinsToCollect)
		{
			Graph->AddNode(Coin);
		}

		RecursiveBackTracker(Graph->GetNodes()[FMath::RandHelper(Graph->GetNodes().Num()-1)]->Vertex);
		Graph->ResetVisitedNodes();
		AddRandomConnections();

	}
}

void AGraphGenerator::AddRandomConnections()
{
	TArray<AActor*> NearestCoins;

	for(auto &CurrentNode: Graph->GetNodes())
	{
		CurrentNode->Vertex->GetOverlappingActors(NearestCoins, ACoin::StaticClass());
		for (auto &Neighbor : NearestCoins)
		{
			FGNode* NextNode = Graph->GetNodeByVertex(Neighbor);
			if (!Graph->HasRelation(CurrentNode, NextNode) && FMath::RandRange(0.f, 1.f) < GraphDensity)
			{
				Graph->AddRelation(CurrentNode, NextNode);
				Graph->AddRelation(NextNode, CurrentNode);
				DrawDebugLine(GetWorld(), CurrentNode->Vertex->GetActorLocation(), NextNode->Vertex->GetActorLocation(), FColor::White, true, 0.f);
			}
		}
	}
}

// Called when the game starts or when spawned
void AGraphGenerator::BeginPlay()
{
	Super::BeginPlay();

	//SpawnGraph and Generate
	FTransform SpawnLocation = FTransform();
	Graph = Cast<AGraph>(GetWorld()->SpawnActor(AGraph::StaticClass(), &SpawnLocation));
	
}

void AGraphGenerator::RecursiveBackTracker(AActor* Start)
{
	TArray<AActor*> NearestCoins;
	TArray<FGNode*> Stack;

	// Start Recursive Back-tracker
	FGNode* CurrentNode = Graph->GetNodeByVertex(Start);
	Graph->Visit(nullptr, CurrentNode);

	Stack.Add(CurrentNode);

	while (Stack.Num())
	{
		CurrentNode = Stack.Last(0);

		//Select one of free neighbors (the first one in the list)
		FGNode* NextNode = nullptr;
		CurrentNode->Vertex->GetOverlappingActors(NearestCoins, ACoin::StaticClass());
		for (auto &Neighbor : NearestCoins)
		{
			if (!Graph->GetNodeByVertex(Neighbor)->bIsVisited)
			{
				NextNode = Graph->GetNodeByVertex(Neighbor);
				break;
			}
		}

		if (NextNode)
		{
			Graph->AddRelation(CurrentNode, NextNode);
			Graph->AddRelation(NextNode, CurrentNode);
			DrawDebugLine(GetWorld(), CurrentNode->Vertex->GetActorLocation(), NextNode->Vertex->GetActorLocation(), FColor::White, true, 0.f);
			Graph->Visit(CurrentNode, NextNode);
			Stack.Add(NextNode);
		}
		else
		{
		Stack.RemoveAt(Stack.Num() - 1);
		}
	}

	return;
}

// Called every frame
void AGraphGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
