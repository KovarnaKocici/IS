// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Graph.generated.h"

/**
 * 
 */

USTRUCT()
struct NAVMESHOVERRIDE_API FGNode
{
	GENERATED_BODY()

		UPROPERTY()
		bool bIsVisited;

	UPROPERTY()
		AActor* Edge;

		TArray<FGNode*> ConnectedNodes;
};

UCLASS()
class NAVMESHOVERRIDE_API UGraph : public UObject
{
	GENERATED_BODY()
	
public:
	TArray<FGNode*> GetRelations() { return Nodes; }
	//If edge is in Graph return its reference, if not - add it at first
	FGNode* AddNode(AActor* Edge);
	void AddRelation(FGNode* NodeL, FGNode* NodeR);
	void AddRelation(AActor* EdgeL, AActor* EdgeR);
	void Visit(FGNode* Node);
	bool IsEdgeInGraph(AActor* Edge);
	FGNode* GetNodeByEdge(AActor* Edge);

	static TArray<AActor*> DFS(UGraph* G, AActor* Start, AActor* End);
	static TArray<AActor*> BFS(UGraph* G, AActor* Start, AActor* End);
private:
	TArray<FGNode*> Nodes;
	
	
};