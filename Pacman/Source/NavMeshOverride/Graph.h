// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/EngineTypes.h"
#include "Graph.generated.h"

/**
 * 
 */

struct FGEdge;
struct FGNode;

USTRUCT()
struct NAVMESHOVERRIDE_API FGNode
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsVisited = false;

	UPROPERTY()
	AActor* Vertex = nullptr;

	float VWeight = -1.f;
	FGNode* CameFrom = nullptr;
	TArray<FGEdge*> ConnectedNodes;
};

USTRUCT()
struct NAVMESHOVERRIDE_API FGEdge
{
	GENERATED_BODY()

	FGNode* Node = nullptr;
	float EWeight = 0.f;
};

UCLASS()
class NAVMESHOVERRIDE_API AGraph : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	float DeltaTime = 0.2f;
	UPROPERTY(BlueprintReadWrite)
	float DelayTime = 1.f;

	AGraph(const FObjectInitializer& ObjectInitializer);
	int GetSize() { return Nodes.Num(); }
	void GenerateGraphFromLevel(bool bIncludeOnlyTargets = false);
	FGEdge* GetEdge(FGNode* From, FGNode* To);
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
	void DrawGraphDebug(const FGNode* CurrentNode, const int& index);
	void PrintPath(const TArray<FGNode*> Path) const;

	TArray<AActor*> DFS(AActor* Start, AActor* End);
	TArray<AActor*> BFS(AActor* Start, AActor* End);
	TArray<AActor*> Dijkstra(AActor* Start, AActor* End);
	TArray<AActor*> AStar(AActor* Start, AActor* End);

protected: 
	TArray<FGNode*> Nodes;
};