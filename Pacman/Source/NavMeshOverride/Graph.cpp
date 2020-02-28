// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Object.h"

#pragma optimize("", off)

FGNode* UGraph::AddNode(AActor* Edge)
{
	FGNode* TNode = nullptr;

	if (!IsEdgeInGraph(Edge))
	{
		TArray<FGNode*> TNodes;
		TNode = new FGNode{ false, Edge, TNodes };
		Nodes.Add(TNode);
	}
	else
	{
		TNode = GetNodeByEdge(Edge);
	}
	return TNode;
}

void UGraph::AddRelation(AActor* EdgeL, AActor* EdgeR)
{

	FGNode* TNodeR = AddNode(EdgeR);
	FGNode* TNodeL = AddNode(EdgeL);
	AddRelation(TNodeL, TNodeR);
}

void UGraph::AddRelation(FGNode* NodeL, FGNode* NodeR)
{
	NodeL->ConnectedNodes.AddUnique(NodeR);
}

void UGraph::Visit(FGNode* Node)
{

	Node->bIsVisited = true;
}

bool UGraph::IsEdgeInGraph(AActor* Edge)
{
	for (auto Node : Nodes)
	{
		if (Node->Edge == Edge)
		{
			return true;
		}
	}
	return false;
}

FGNode* UGraph::GetNodeByEdge(AActor* Edge)
{
	for (auto Node : Nodes)
	{
		if (Node->Edge == Edge)
		{
			return Node;
		}
	}
	return nullptr;
}

TArray<AActor*> UGraph::BFS( UGraph* G, AActor* Start, AActor* End)
{
	TArray<AActor*> Path;
	TArray<FGNode*> Queue;
	FGNode* StartNode = G->GetNodeByEdge(Start);

	int i = 0;

		// Mark the current node as visited and enqueue it 
		G->Visit(StartNode);
		Queue.Add(StartNode);

		while(Queue.Num())
		{
			// Dequeue a vertex from queue and print it 
			StartNode = Queue[0];
			Path.Add(StartNode->Edge);

			if (StartNode->Edge == End)
			{
				return Path;
			}

			DrawDebugString(Start->GetWorld(), StartNode->Edge->GetActorLocation(), FString::Printf(TEXT("%i"), i), nullptr, FColor::White, -1.f, true);
			i++;
			Queue.RemoveAt(0);

			// Get all adjacent vertices of the dequeued 
			// vertex s. If a adjacent has not been visited,  
			// then mark it visited and enqueue it 
			FColor LayerColor = FColor::MakeRandomColor();
			for (auto Node : StartNode->ConnectedNodes)
			{
				if (!Node->bIsVisited)
				{
					G->Visit(Node);
					DrawDebugLine(Start->GetWorld(), StartNode->Edge->GetActorLocation(), Node->Edge->GetActorLocation(), LayerColor, true, 0.f);
					Queue.Add(Node);
				}
			}
		}
		return Path;

}

TArray< AActor*> UGraph::DFS( UGraph* G, AActor* Start, AActor* End)
{
	// Create a queue for BFS 
	TArray<AActor*> Queue;
	return Queue;
}



