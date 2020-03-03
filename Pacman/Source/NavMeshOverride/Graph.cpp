// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Object.h"
#include "TimerManager.h"
#include "UObjectBase.h"

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
		if (!ConnectedNode->bIsVisited)
		{
			NotVisited.Add(*ConnectedNode);
		}
	}

	return NotVisited;
}

FGNode* UGraph::AddNode(AActor* Vertex)
{
	FGNode* TNode = nullptr;

	if (!IsVertexInGraph(Vertex))
	{
		TArray<FGNode*> TNodes;
		TNode = new FGNode{ false, Vertex, TNodes };
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
	NodeL->ConnectedNodes.AddUnique(NodeR);
}

void UGraph::Visit(FGNode* Node)
{
	if(Node)
	{
		Node->bIsVisited = true;
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

void UGraph::PrintPath(const TArray<AActor*> Path) const
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

TArray<AActor*> UGraph::BFS( AActor* Start, AActor* End)
{
	const float Delta = 0.2f;
	float Delay = 1.f;

	TArray<AActor*> Path;
	TArray<FGNode*> Queue;
	FGNode* StartNode = GetNodeByVertex(Start);

	Visit(StartNode);
	Queue.Add(StartNode);

	int i = 0;
	while (Queue.Num())
	{
		StartNode = Queue[0];
		Path.Add(StartNode->Vertex);
		DrawDebugString(StartNode->Vertex->GetWorld(), StartNode->Vertex->GetActorLocation(), FString::Printf(TEXT("%i"), i), nullptr, FColor::White, -1.f, true);

		Queue.RemoveAt(0);

		if (StartNode->Vertex == End)
		{
			break;
		}

		FTimerHandle DrawTimerHandle;
		FTimerDelegate DrawTimerDelegate;

		DrawTimerDelegate.BindUFunction(this, FName(TEXT("DrawEdges")), *StartNode, GetNotVisitedNodes(StartNode));
		StartNode->Vertex->GetWorld()->GetTimerManager().SetTimer(DrawTimerHandle, DrawTimerDelegate, Delay, false);

		if (StartNode->ConnectedNodes.Num()) 
		{
			for (auto Node : StartNode->ConnectedNodes)
			{
				Delay += Delta;

				if (!Node->bIsVisited)
				{
					Visit(Node);
					Queue.Add(Node);
				}
			}
		}

		i++;
	}
	PrintPath(Path);
	return Path;

}

TArray< AActor*> UGraph::DFS(AActor* Start, AActor* End)
{
	const float Delta = 0.2f;
	float Delay = 1.f;

	TArray<AActor*> FullPath;
	TArray<AActor*> PathToTarget;
	TArray<FGNode*> Stack;
	FGNode* StartNode = GetNodeByVertex(Start);

	Visit(StartNode);
	Stack.Add(StartNode);
	PathToTarget.Add(StartNode->Vertex);

	int i = 0;
	while (Stack.Num())
	{
		StartNode = Stack.Last(0);
		FullPath.Add(StartNode->Vertex);

		Stack.RemoveAt(Stack.Num()-1);

		if (StartNode->Vertex == End)
		{
			break;
		}

		if (StartNode->ConnectedNodes.Num())
		{
			bool AnyVisited = false;
			for (auto Node : StartNode->ConnectedNodes)
			{
				if (!Node->bIsVisited)
				{
					Visit(Node);

					Stack.Add(Node);
					FullPath.Add(Node->Vertex);
					PathToTarget.Add(Node->Vertex);

					AnyVisited = true;
					break;
				}
			}
			if (!AnyVisited)
			{
				PathToTarget.RemoveAt(PathToTarget.Num() - 1);
			}
		}

		i++;
	}
	PrintPath(FullPath);
	return PathToTarget;
}

