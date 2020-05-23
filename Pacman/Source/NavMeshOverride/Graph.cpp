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

FGEdge* AGraph::GetEdge(FGNode* From, FGNode* To)
{
	FGEdge* TEdge = nullptr;

	for (auto Edge : From->ConnectedNodes)
	{
		if (Edge->TargetNode == To)
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

	for (const auto &ConnectedNode : Node->ConnectedNodes)
	{
		if (!ConnectedNode->TargetNode->bIsVisited)
		{
			NotVisited.Add(*(ConnectedNode->TargetNode));
		}
	}

	return NotVisited;
}

int AGraph::GetNumNotVisitedNodes() const
{
	int Num = 0;

	for (auto const &Node: Nodes)
	{
		if (!Node->bIsVisited)
		{
			++Num;
		}
	}

	return Num;
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

	NodeL->ConnectedNodes.AddUnique(new FGEdge{ NodeR, Price });
}

bool AGraph::HasRelation(FGNode* NodeL, FGNode* NodeR)
{
	for (const auto &Edge : NodeL->ConnectedNodes)
	{
		if (Edge->TargetNode == NodeR)
		{
			return true;
		}
	}
	return false;

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

void AGraph::ResetVisitedNodes()
{
	for (auto &Node : Nodes)
	{
		Node->bIsVisited = false;
		Node->CameFrom = nullptr;
		//Node->VWeight = 0.0f;
	}
}

void AGraph::DrawGraphDebug(const FGNode* CurrentNode, const int& index, float Time = -1.f) 
{
	DrawDebugString(GetWorld(), CurrentNode->Vertex->GetActorLocation(), FString::Printf(TEXT("%i"), index), nullptr, FColor::White, Time, true);

	FTimerHandle DrawTimerHandle;
	FTimerDelegate DrawTimerDelegate;

	DrawTimerDelegate.BindUFunction(this, FName(TEXT("DrawEdges")), *CurrentNode, GetNotVisitedNodes(CurrentNode), Time);
	GetWorld()->GetTimerManager().SetTimer(DrawTimerHandle, DrawTimerDelegate, DelayTime, false);
	DelayTime += DeltaTime;
}

void AGraph::DrawEdges(const FGNode From, const TArray<FGNode> To, float Time) const
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
		DrawDebugLine(GetWorld(), From.Vertex->GetActorLocation(), Node.Vertex->GetActorLocation(), LayerColor, true, Time);
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
	TArray<FGNode*> FullPath;
	TArray<FGNode*> Queue;
	FGNode* CurrentNode = GetNodeByVertex(Start);

	if (!Start || !CurrentNode)
	{
		return TArray<AActor*>();
	}

	Visit(nullptr, CurrentNode);
	Queue.Add(CurrentNode);

	int i = 0;
	while (Queue.Num())
	{
		CurrentNode = Queue[0];
		FullPath.Add(CurrentNode);
		Queue.RemoveAt(0);
		
		DrawGraphDebug(CurrentNode, i);

		if (CurrentNode->Vertex == End)
		{
			break;
		}

		if (CurrentNode->ConnectedNodes.Num()) 
		{
			for (auto &OutEdge : CurrentNode->ConnectedNodes)
			{
				if (!OutEdge->TargetNode->bIsVisited)
				{
					Visit(CurrentNode, OutEdge->TargetNode);
					Queue.Add(OutEdge->TargetNode);
				}
			}
		}

		i++;
	}
	PrintPath(FullPath);

	return BacktrackPath(FullPath.Last());

}

TArray<AActor*> AGraph::DFS(AActor* Start, AActor* End)
{
	TArray<FGNode*> FullPath;
	TArray<FGNode*> Stack;
	FGNode* CurrentNode = GetNodeByVertex(Start);

	if (!Start || !CurrentNode)
	{
		return TArray<AActor*>();
	}

	Visit(nullptr, CurrentNode);
	Stack.Add(CurrentNode);

	int i = 0;
	while (Stack.Num())
	{
		CurrentNode = Stack.Last(0);
		FullPath.Add(CurrentNode);
		Stack.RemoveAt(Stack.Num()-1);

		DrawGraphDebug(CurrentNode, i);

		if (CurrentNode->Vertex == End)
		{
			break;
		}

		if (CurrentNode->ConnectedNodes.Num())
		{
			for (auto &OutEdge : CurrentNode->ConnectedNodes)
			{
				if (!OutEdge->TargetNode->bIsVisited)
				{
					Visit(CurrentNode, OutEdge->TargetNode);

					Stack.Add(OutEdge->TargetNode);
					FullPath.Add(OutEdge->TargetNode);
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
	TArray<FGNode*> FullPath;
	TArray<FGNode*> PriorityQueue;
	FGNode* CurrentNode = GetNodeByVertex(Start);

	if (!Start || !CurrentNode)
	{
		return TArray<AActor*>();
	}

	PriorityQueue.Add(CurrentNode);
	CurrentNode->VWeight = 0.f;

	int i = 0;
	while (PriorityQueue.Num())
	{
		CurrentNode = PriorityQueue[0];
		FullPath.Add(CurrentNode);
		PriorityQueue.RemoveAt(0);

		DrawGraphDebug(CurrentNode, i);

		if (CurrentNode->Vertex == End)
		{
			break;
		}

		if (CurrentNode->ConnectedNodes.Num())
		{
			int NewWeight = 0.f;
			for (auto &OutEdge : CurrentNode->ConnectedNodes)
			{
				NewWeight = CurrentNode->VWeight + GetEdge(CurrentNode, OutEdge->TargetNode)->EWeight;
				if (!OutEdge->TargetNode->bIsVisited && (OutEdge->TargetNode->VWeight == -1.f || NewWeight < OutEdge->EWeight))
				{
					OutEdge->TargetNode->VWeight = NewWeight;

					//Add to Queue by priority large->low
					if (!PriorityQueue.Num())
					{
						PriorityQueue.Add(OutEdge->TargetNode);
						Visit(CurrentNode, OutEdge->TargetNode);
					}
					else
					{
						for (int i = 0; i < PriorityQueue.Num(); i++)
						{
							if (NewWeight <= PriorityQueue[i]->VWeight)
							{
								PriorityQueue.Insert(OutEdge->TargetNode, i);
								Visit(CurrentNode, OutEdge->TargetNode);
								break;
							}
							if (i == PriorityQueue.Num() - 1 && NewWeight > PriorityQueue[i]->VWeight)
							{
								PriorityQueue.Add(OutEdge->TargetNode);
								Visit(CurrentNode, OutEdge->TargetNode);
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
	TArray<FGNode*> FullPath;
	TArray<FGNode*> PriorityQueue;
	FGNode* CurrentNode = GetNodeByVertex(Start);

	if (!Start || !CurrentNode)
	{
		return TArray<AActor*>();
	}

	//Manhattan distance on a square grid
	auto Heuristic = [](FVector A, FVector B) {
		return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y);
	};

	PriorityQueue.Add(CurrentNode);
	CurrentNode->VWeight = 0.f;

	int i = 0;
	while (PriorityQueue.Num())
	{
		CurrentNode = PriorityQueue[0];
		FullPath.Add(CurrentNode);
		PriorityQueue.RemoveAt(0);

		DrawGraphDebug(CurrentNode, i);

		if (CurrentNode->Vertex == End)
		{
			break;
		}

		if (CurrentNode->ConnectedNodes.Num())
		{
			int NewWeight = 0.f;
			for (auto &OutEdge : CurrentNode->ConnectedNodes)
			{
				NewWeight = Heuristic(CurrentNode->Vertex->GetActorLocation(), End->GetActorLocation()); 
				if (!OutEdge->TargetNode->bIsVisited && (OutEdge->TargetNode->VWeight == -1.f || NewWeight < OutEdge->EWeight))
				{
					OutEdge->TargetNode->VWeight = NewWeight;

					//Add to Queue by priority large->low
					if (!PriorityQueue.Num())
					{
						PriorityQueue.Add(OutEdge->TargetNode);
						Visit(CurrentNode, OutEdge->TargetNode);
					}
					else
					{
						for (int i = 0; i < PriorityQueue.Num(); i++)
						{
							if (NewWeight <= PriorityQueue[i]->VWeight)
							{
								PriorityQueue.Insert(OutEdge->TargetNode, i);
								Visit(CurrentNode, OutEdge->TargetNode);
								break;
							}
							if (i == PriorityQueue.Num() - 1 && NewWeight > PriorityQueue[i]->VWeight)
							{
								PriorityQueue.Add(OutEdge->TargetNode);
								Visit(CurrentNode, OutEdge->TargetNode);
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

TArray<TArray<AActor*>> AGraph::GetAllPathsOfLength(AActor* Start, int SearchDepth)
{
	TArray<TArray<AActor*>> Paths;
	TArray<FGNode*> Stack;
	FGNode* CurrentNode = GetNodeByVertex(Start);

	if (!Start || !CurrentNode)
	{
		return TArray<TArray<AActor*>>();
	}

	Visit(nullptr, CurrentNode);
	CurrentNode->VWeight = 0;
	Stack.Add(CurrentNode);

	int CurrentDepth = 0; //including Start
	int i = 0;
	while (Stack.Num())
	{

		CurrentNode = Stack.Last(0);
		Stack.RemoveAt(Stack.Num() - 1);

		DrawGraphDebug(CurrentNode, CurrentDepth, 1.f);

		if (CurrentNode->VWeight == SearchDepth || !GetNotVisitedNodes(CurrentNode).Num())
		{
			Paths.Add(TArray<AActor*>(BacktrackPath(CurrentNode)));
		}
		else
		{
			if (CurrentNode->ConnectedNodes.Num())
			{
				CurrentDepth = CurrentNode->VWeight + 1;

				for (auto &OutEdge : CurrentNode->ConnectedNodes)
				{
					if (!OutEdge->TargetNode->bIsVisited)
					{
						Visit(CurrentNode, OutEdge->TargetNode);
						OutEdge->TargetNode->VWeight = CurrentDepth; 
						Stack.Add(OutEdge->TargetNode);
					}
				}
			}
		}
		i++;
	}

	ResetVisitedNodes();
	return Paths;

}
