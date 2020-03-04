// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Object.h"
#include "TimerManager.h"
#include "UObjectBase.h"
#include "AI/Navigation/NavigationSystem.h"

#pragma optimize("", off)

UWorld* UGraph::GetWorld() const
{
	if (GIsEditor && !GIsPlayInEditorWorld)
	{
		return nullptr;
	}
	else if (GetOuter())
	{
		return GetOuter()->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

TArray<FGNode> UGraph::GetNotVisitedNodes(const FGNode* Node) const
{

	TArray<FGNode> NotVisited;

	for (auto ConnectedNode : Node->ConnectedNodes)
	{
		if (!ConnectedNode->Connection->bIsVisited)
		{
			NotVisited.Add(*(ConnectedNode->Connection));
		}
	}

	return NotVisited;
}

FGNode* UGraph::AddNode(AActor* Vertex)
{
	FGNode* TNode = nullptr;

	if (!IsVertexInGraph(Vertex))
	{
		TArray<FGEdge*> TEdges;
		TNode = new FGNode{ false, Vertex, nullptr, TEdges };
		Nodes.Add(TNode);
	}
	else
	{
		TNode = GetNodeByVertex(Vertex);
	}
	return TNode;
}

void UGraph::AddRelation(AActor* VertexL, AActor* VertexR)
{
	FGNode* TNodeR = AddNode(VertexR);
	FGNode* TNodeL = AddNode(VertexL);
	AddRelation(TNodeL, TNodeR);
}

void UGraph::AddRelation(FGNode* NodeL, FGNode* NodeR)
{
	float Distance = 1.f;
	UNavigationSystem* const NavSys = NodeL->Vertex->GetWorld()->GetNavigationSystem();
	if (NavSys)
	{
		NavSys->GetPathLength(NodeL->Vertex->GetActorLocation(), NodeR->Vertex->GetActorLocation(), Distance);
	}

	NodeL->ConnectedNodes.AddUnique(new FGEdge{ NodeR, Distance });
}

void UGraph::Visit(FGNode* Parent, FGNode* Child)
{
	if(Child)
	{
		Child->bIsVisited = true;
		Child->CameFrom = Parent;
	}
}

bool UGraph::IsVertexInGraph(AActor* Vertex)
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

FGNode* UGraph::GetNodeByVertex(AActor* Vertex)
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

TArray<AActor*> UGraph::BacktrackPath(FGNode* Node)
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

void UGraph::DrawEdges(const FGNode From, const TArray<FGNode> To) const
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
		DrawDebugLine(From.Vertex->GetWorld(), From.Vertex->GetActorLocation(), Node.Vertex->GetActorLocation(), LayerColor, true, 0.f);
	}
}

void UGraph::PrintPath(const TArray<FGNode*> Path) const
{
	if (!Path.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("PATH DON`T EXIST"));
	}
	//for (int i = 0; i < Path.Num() - 1; i++)
	//{
		//DrawDebugLine(Path[i]->GetWorld(), Path[i]->GetActorLocation(), Path[i+1]->GetActorLocation(), FColor::Black, true, 0.f);
	//}
}

TArray<AActor*> UGraph::Dijkstra(AActor* Start)
{

}

TArray<AActor*> UGraph::BFS( AActor* Start, AActor* End)
{
	const float Delta = 0.2f;
	float Delay = 1.f;

	TArray<FGNode*> FullPath;
	TArray<FGNode*> Queue;
	FGNode* StartNode = GetNodeByVertex(Start);

	Visit(nullptr, StartNode);
	Queue.Add(StartNode);

	int i = 0;
	while (Queue.Num())
	{
		StartNode = Queue[0];
		FullPath.Add(StartNode);
		Queue.RemoveAt(0);
		
		DrawDebugString(StartNode->Vertex->GetWorld(), StartNode->Vertex->GetActorLocation(), FString::Printf(TEXT("%i"), i), nullptr, FColor::White, -1.f, true);

		FTimerHandle DrawTimerHandle;
		FTimerDelegate DrawTimerDelegate;

		DrawTimerDelegate.BindUFunction(this, FName(TEXT("DrawEdges")), *StartNode, GetNotVisitedNodes(StartNode));
		StartNode->Vertex->GetWorld()->GetTimerManager().SetTimer(DrawTimerHandle, DrawTimerDelegate, Delay, false);
		Delay += Delta;

		if (StartNode->Vertex == End)
		{
			break;
		}

		if (StartNode->ConnectedNodes.Num()) 
		{
			for (auto Node : StartNode->ConnectedNodes)
			{
				if (!Node->Connection->bIsVisited)
				{
					Visit(StartNode, Node->Connection);
					Queue.Add(Node->Connection);
				}
			}
		}

		i++;
	}
	PrintPath(FullPath);

	return BacktrackPath(FullPath.Last());

}

TArray< AActor*> UGraph::DFS(AActor* Start, AActor* End)
{
	const float Delta = 0.2f;
	float Delay = 1.f;

	TArray<FGNode*> FullPath;
	TArray<FGNode*> Stack;
	FGNode* StartNode = GetNodeByVertex(Start);

	Visit(nullptr, StartNode);
	Stack.Add(StartNode);

	int i = 0;
	while (Stack.Num())
	{
		StartNode = Stack.Last(0);
		FullPath.Add(StartNode);
		Stack.RemoveAt(Stack.Num()-1);

		DrawDebugString(StartNode->Vertex->GetWorld(), StartNode->Vertex->GetActorLocation(), FString::Printf(TEXT("%i"), i), nullptr, FColor::White, -1.f, true);

		FTimerHandle DrawTimerHandle;
		FTimerDelegate DrawTimerDelegate;

		DrawTimerDelegate.BindUFunction(this, FName(TEXT("DrawEdges")), *StartNode, GetNotVisitedNodes(StartNode));
		StartNode->Vertex->GetWorld()->GetTimerManager().SetTimer(DrawTimerHandle, DrawTimerDelegate, Delay, false);
		Delay += Delta;

		if (StartNode->Vertex == End)
		{
			break;
		}

		if (StartNode->ConnectedNodes.Num())
		{
			for (auto Node : StartNode->ConnectedNodes)
			{

				if (!Node->Connection->bIsVisited)
				{
					Visit(StartNode, Node->Connection);

					Stack.Add(Node->Connection);
					FullPath.Add(Node->Connection);
				}
			}
		}

		i++;
	}

	PrintPath(FullPath);
	return BacktrackPath(FullPath.Last());
}

