// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "Graph.generated.h"

/**
 * 
 */
USTRUCT()
struct NAVMESHOVERRIDE_API FGEdge
{
	GENERATED_BODY()

	FGNode* Connection = nullptr;
	float Weight = 1.f;
};

USTRUCT()
struct NAVMESHOVERRIDE_API FGNode
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsVisited = false;;

	UPROPERTY()
	AActor* Vertex = nullptr;

	FGNode* CameFrom = nullptr;
	TArray<FGEdge*> ConnectedNodes;
};

UCLASS()
class NAVMESHOVERRIDE_API UGraph : public UObject
{
	GENERATED_BODY()
	
public:
	virtual UWorld* GetWorld() const override;

	TArray<FGNode*> GetRelations() { return Nodes; }
	TArray<FGNode> GetNotVisitedNodes(const FGNode* Node) const;
	//If Vertex is in Graph return its reference, if not - add it at first
	FGNode* AddNode(AActor* Vertex);
	void AddRelation(FGNode* NodeL, FGNode* NodeR);
	void AddRelation(AActor* VertexL, AActor* VertexR);
	void Visit(FGNode* Parent, FGNode* Child);
	bool IsVertexInGraph(AActor* Vertex);
	FGNode* GetNodeByVertex(AActor* Vertex);
	TArray<AActor*> BacktrackPath(FGNode* Node);

	UFUNCTION()
	void DrawEdges(const FGNode From, const TArray<FGNode> To) const;

	void PrintPath(const TArray<FGNode*> Path) const;

	TArray<AActor*> Dijkstra(AActor* Start);
	TArray<AActor*> DFS(AActor* Start, AActor* End);
	TArray<AActor*> BFS(AActor* Start, AActor* End);

protected: 
	TArray<FGNode*> Nodes;
};