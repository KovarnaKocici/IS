// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Object.h"
#include "TimerManager.h"
#include "UObjectBase.h"
#include "AI/Navigation/NavigationSystem.h"
#include "NavMeshOverrideGameMode.h"
#include "Pacman.h"
#include "Kismet/GameplayStatics.h"
#include "Coin.h"
#include "LogMacros.h"

#pragma optimize("", off)

AGraph::AGraph(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void AGraph::GenerateGraphFromLevel(bool bIncludeOnlyTargets)
{
	ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM || !GM->CoinsToCollect.Num())
	{
		return;
	}

	if (GM->CoinsToCollect.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Was found %i actors"), GM->CoinsToCollect.Num());
	}

	//Initialize Graph
	TArray<AActor*> Vertexes;

	//Fins all targets
	//Find PlayerAI and add its pawn
	TArray<AActor*> Players;
	TSubclassOf<APacman> classToFind;
	classToFind = APacman::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, Players);
	if (Players.Num())
	{
		Vertexes.Add(Cast<AActor>(Players[0]));
	}

	for (auto Coin : GM->CoinsToCollect)
	{
		if (Coin->bIsTarget && bIncludeOnlyTargets)
		{
			Vertexes.Add(Cast<AActor>(Coin));
			AddNode(Vertexes.Last(0));
		}
		else if (!bIncludeOnlyTargets)
		{
			Vertexes.Add(Cast<AActor>(Coin));
			AddNode(Vertexes.Last(0));
		}
	}


	TArray<AActor*> NearestCoins;

	for (auto ActorL : Vertexes)
	{
		if (bIncludeOnlyTargets)
		{
			NearestCoins = Vertexes;
		}
		else
		{
			ActorL->GetOverlappingActors(NearestCoins, ACoin::StaticClass());
		}

		for (auto ActorR : NearestCoins)
		{
			if (ActorL != ActorR /*&& FMath::RandRange(0.f, 1.f) < GraphDensity*/)
			{
				AddRelation(ActorL, ActorR);
				AddRelation(ActorR, ActorL);
				DrawDebugLine(GetWorld(), ActorL->GetActorLocation(), ActorR->GetActorLocation(), FColor::White, true, 0.f);
			}
		}
	}
}

FGEdge* AGraph::GetEdge(FGNode* From, FGNode* To)
{
	FGEdge* TEdge = nullptr;

	for (auto Edge : From->ConnectedNodes)
	{
		if (Edge->Node == To)
		{
			TEdge = Edge;
			break;
		}
	}
	return TEdge;
}

TArray<FGNode> AGraph::GetNotVisitedNodes(const FGNode* Node) const
{

	TArray<FGNode> NotVisited;

	for (auto ConnectedNode : Node->ConnectedNodes)
	{
		if (!ConnectedNode->Node->bIsVisited)
		{
			NotVisited.Add(*(ConnectedNode->Node));
		}
	}

	return NotVisited;
}

FGNode* AGraph::AddNode(AActor* Vertex)
{
	FGNode* TNode = nullptr;

	if (!IsVertexInGraph(Vertex))
	{
		TArray<FGEdge*> TEdges;
		TNode = new FGNode{ false, Vertex, -1.f, nullptr, TEdges };
		Nodes.Add(TNode);
	}
	else
	{
		TNode = GetNodeByVertex(Vertex);
	}
	return TNode;
}

void AGraph::AddRelation(AActor* VertexL, AActor* VertexR)
{
	FGNode* TNodeR = AddNode(VertexR);
	FGNode* TNodeL = AddNode(VertexL);
	AddRelation(TNodeL, TNodeR);
}

void AGraph::AddRelation(FGNode* NodeL, FGNode* NodeR)
{
	float Price = 1.f;
	if (Cast<ACoin>(NodeR->Vertex))
	{
		Price = Cast<ACoin>(NodeR->Vertex)->Price;

		if (Price != 1.f)
		{
			FVector Location = (NodeL->Vertex->GetActorLocation() + NodeR->Vertex->GetActorLocation()) / 2.f;
			DrawDebugString(GetWorld(), FVector(Location.X, Location.Y, NodeR->Vertex->GetActorLocation().Z), FString::SanitizeFloat(Price), nullptr, FColor::Red, -1.f, true);
		}
	}
	//UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();
	//if (NavSys)
	//{
	//	NavSys->GetPathLength(NodeL->Vertex->GetActorLocation(), NodeR->Vertex->GetActorLocation(), Distance);
	//}

	NodeL->ConnectedNodes.AddUnique(new FGEdge{ NodeR, Price });
}

void AGraph::Visit(FGNode* Parent, FGNode* Child)
{
	if(Child)
	{
		Child->bIsVisited = true;
		Child->CameFrom = Parent;
	}
}

bool AGraph::IsVertexInGraph(AActor* Vertex)
{
	for (auto Node : Nodes)
	{
		if (Node->Vertex == Vertex)
		{
			return true;
		}
	}
	return false;
}

FGNode* AGraph::GetNodeByVertex(AActor* Vertex)
{
	for (auto Node : Nodes)
	{
		if (Node->Vertex == Vertex)
		{
			return Node;
		}
	}
	return nullptr;
}

TArray<AActor*> AGraph::BacktrackPath(FGNode* Node)
{
	TArray<AActor*> Path;
	if (Node)
	{
		Path.Add(Node->Vertex);
		FGNode* TNode = Node->CameFrom;

		while (TNode != nullptr)
		{
			Path.Add(TNode->Vertex);
			TNode = TNode->CameFrom;
		}
	}

	return Path;
}

void AGraph::DrawEdges(const FGNode From, const TArray<FGNode> To) const
{
	if (!To.Num())
	{
		return;
	}

	FColor LayerColor = FColor::MakeRandomColor();

	while (LayerColor == FColor::White)
	{
		LayerColor = FColor::MakeRandomColor();
	}

	for (auto Node : To)
	{
		DrawDebugLine(GetWorld(), From.Vertex->GetActorLocation(), Node.Vertex->GetActorLocation(), LayerColor, true, 0.f);
	}
}

void AGraph::PrintPath(const TArray<FGNode*> Path) const
{
	if (!Path.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("PATH DON`T EXIST"));
	}
	//for (int i = 0; i < Path.Num() - 1; i++)
	//{
		//DrawDebugLine(GetWorld(), Path[i]->GetActorLocation(), Path[i+1]->GetActorLocation(), FColor::Black, true, 0.f);
	//}
}

TArray<AActor*> AGraph::BFS( AActor* Start, AActor* End)
{
	const float Delta = 0.2f;
	float Delay = 1.f;

	TArray<FGNode*> FullPath;
	TArray<FGNode*> Queue;
	FGNode* CurrentNode = GetNodeByVertex(Start);

	Visit(nullptr, CurrentNode);
	Queue.Add(CurrentNode);

	int i = 0;
	while (Queue.Num())
	{
		CurrentNode = Queue[0];
		FullPath.Add(CurrentNode);
		Queue.RemoveAt(0);
		
		DrawDebugString(GetWorld(), CurrentNode->Vertex->GetActorLocation(), FString::Printf(TEXT("%i"), i), nullptr, FColor::White, -1.f, true);

		FTimerHandle DrawTimerHandle;
		FTimerDelegate DrawTimerDelegate;

		DrawTimerDelegate.BindUFunction(this, FName(TEXT("DrawEdges")), *CurrentNode, GetNotVisitedNodes(CurrentNode));
		GetWorld()->GetTimerManager().SetTimer(DrawTimerHandle, DrawTimerDelegate, Delay, false);
		Delay += Delta;

		if (CurrentNode->Vertex == End)
		{
			break;
		}

		if (CurrentNode->ConnectedNodes.Num()) 
		{
			for (auto OutEdge : CurrentNode->ConnectedNodes)
			{
				if (!OutEdge->Node->bIsVisited)
				{
					Visit(CurrentNode, OutEdge->Node);
					Queue.Add(OutEdge->Node);
				}
			}
		}

		i++;
	}
	PrintPath(FullPath);

	return BacktrackPath(FullPath.Last());

}

TArray< AActor*> AGraph::DFS(AActor* Start, AActor* End)
{
	const float Delta = 0.2f;
	float Delay = 1.f;

	TArray<FGNode*> FullPath;
	TArray<FGNode*> Stack;
	FGNode* CurrentNode = GetNodeByVertex(Start);

	Visit(nullptr, CurrentNode);
	Stack.Add(CurrentNode);

	int i = 0;
	while (Stack.Num())
	{
		CurrentNode = Stack.Last(0);
		FullPath.Add(CurrentNode);
		Stack.RemoveAt(Stack.Num()-1);

		DrawDebugString(CurrentNode->Vertex->GetWorld(), CurrentNode->Vertex->GetActorLocation(), FString::Printf(TEXT("%i"), i), nullptr, FColor::White, -1.f, true);

		FTimerHandle DrawTimerHandle;
		FTimerDelegate DrawTimerDelegate;

		DrawTimerDelegate.BindUFunction(this, FName(TEXT("DrawEdges")), *CurrentNode, GetNotVisitedNodes(CurrentNode));
		GetWorld()->GetTimerManager().SetTimer(DrawTimerHandle, DrawTimerDelegate, Delay, false);
		Delay += Delta;

		if (CurrentNode->Vertex == End)
		{
			break;
		}

		if (CurrentNode->ConnectedNodes.Num())
		{
			for (auto OutEdge : CurrentNode->ConnectedNodes)
			{
				if (!OutEdge->Node->bIsVisited)
				{
					Visit(CurrentNode, OutEdge->Node);

					Stack.Add(OutEdge->Node);
					FullPath.Add(OutEdge->Node);
				}
			}
		}

		i++;
	}

	PrintPath(FullPath);
	return BacktrackPath(FullPath.Last());
}

TArray<AActor*> AGraph::Dijkstra(AActor* Start, AActor* End)
{
	const float Delta = 0.2f;
	float Delay = 1.f;

	TArray<FGNode*> FullPath;
	TArray<FGNode*> PriorityQueue;
	FGNode* CurrentNode = GetNodeByVertex(Start);

	//Visit(nullptr, StartNode);
	PriorityQueue.Add(CurrentNode);
	CurrentNode->VWeight = 0.f;

	int i = 0;
	while (PriorityQueue.Num())
	{
		CurrentNode = PriorityQueue[0];
		FullPath.Add(CurrentNode);
		PriorityQueue.RemoveAt(0);

		DrawDebugString(GetWorld(), CurrentNode->Vertex->GetActorLocation(), FString::Printf(TEXT("%i"), i), nullptr, FColor::White, -1.f, true);

		FTimerHandle DrawTimerHandle;
		FTimerDelegate DrawTimerDelegate;

		DrawTimerDelegate.BindUFunction(this, FName(TEXT("DrawEdges")), *CurrentNode, GetNotVisitedNodes(CurrentNode));
		GetWorld()->GetTimerManager().SetTimer(DrawTimerHandle, DrawTimerDelegate, Delay, false);
		Delay += Delta;

		if (CurrentNode->Vertex == End)
		{
			break;
		}

		if (CurrentNode->ConnectedNodes.Num())
		{
			int NewWeight = 0.f;
			for (auto OutEdge : CurrentNode->ConnectedNodes)
			{
				bool VisitNode = false;
				NewWeight = CurrentNode->VWeight + GetEdge(CurrentNode, OutEdge->Node)->EWeight;
				if (OutEdge->Node->VWeight == -1.f || NewWeight < OutEdge->EWeight)
				{
					OutEdge->Node->VWeight = NewWeight;

					//Add to Queue by priority large->low
					if (!PriorityQueue.Num())
					{
						PriorityQueue.Add(OutEdge->Node);
						Visit(CurrentNode, OutEdge->Node);
					}
					else
					{
						for (int i = 0; i < PriorityQueue.Num(); i++)
						{
							if (NewWeight <= PriorityQueue[i]->VWeight)
							{
								PriorityQueue.Insert(OutEdge->Node, i);
								Visit(CurrentNode, OutEdge->Node);
								break;
							}
							if (i == PriorityQueue.Num() - 1 && NewWeight > PriorityQueue[i]->VWeight)
							{
								PriorityQueue.Add(OutEdge->Node);
								Visit(CurrentNode, OutEdge->Node);
							}
						}
					}
				}
			}
		}

		i++;
	}
	PrintPath(FullPath);

	return BacktrackPath(FullPath.Last());
}

TArray<AActor*> AGraph::AStar(AActor* Start, AActor* End)
{
	const float Delta = 0.2f;
	float Delay = 1.f;

	TArray<FGNode*> FullPath;
	TArray<FGNode*> PriorityQueue;
	FGNode* CurrentNode = GetNodeByVertex(Start);

	//Visit(nullptr, StartNode);
	PriorityQueue.Add(CurrentNode);
	CurrentNode->VWeight = 0.f;

	int i = 0;
	while (PriorityQueue.Num())
	{
		CurrentNode = PriorityQueue[0];
		FullPath.Add(CurrentNode);
		PriorityQueue.RemoveAt(0);

		DrawDebugString(GetWorld(), CurrentNode->Vertex->GetActorLocation(), FString::Printf(TEXT("%i"), i), nullptr, FColor::White, -1.f, true);

		FTimerHandle DrawTimerHandle;
		FTimerDelegate DrawTimerDelegate;

		DrawTimerDelegate.BindUFunction(this, FName(TEXT("DrawEdges")), *CurrentNode, GetNotVisitedNodes(CurrentNode));
		GetWorld()->GetTimerManager().SetTimer(DrawTimerHandle, DrawTimerDelegate, Delay, false);
		Delay += Delta;

		if (CurrentNode->Vertex == End)
		{
			break;
		}

		if (CurrentNode->ConnectedNodes.Num())
		{
			int NewWeight = 0.f;
			for (auto OutEdge : CurrentNode->ConnectedNodes)
			{
				bool VisitNode = false;
				NewWeight = CurrentNode->VWeight + GetEdge(CurrentNode, OutEdge->Node)->EWeight;
				if (OutEdge->Node->VWeight == -1.f || NewWeight < OutEdge->EWeight)
				{
					OutEdge->Node->VWeight = NewWeight;

					//Add to Queue by priority large->low
					if (!PriorityQueue.Num())
					{
						PriorityQueue.Add(OutEdge->Node);
						Visit(CurrentNode, OutEdge->Node);
					}
					else
					{
						for (int i = 0; i < PriorityQueue.Num(); i++)
						{
							if (NewWeight <= PriorityQueue[i]->VWeight)
							{
								PriorityQueue.Insert(OutEdge->Node, i);
								Visit(CurrentNode, OutEdge->Node);
								break;
							}
							if (i == PriorityQueue.Num() - 1 && NewWeight > PriorityQueue[i]->VWeight)
							{
								PriorityQueue.Add(OutEdge->Node);
								Visit(CurrentNode, OutEdge->Node);
							}
						}
					}
				}
			}
		}

		i++;
	}
	PrintPath(FullPath);

	return BacktrackPath(FullPath.Last());
}
