#pragma once

//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "Graph.generated.h"

//USTRUCT()
//struct NAVMESHOVERRIDE_API FGNode
//{
//	GENERATED_BODY()
//
//	UPROPERTY()
//	bool bIsVisited;
//
//	UPROPERTY()
//	AActor* Edge;
//
//	UPROPERTY()
//	TArray<AActor*> Connections;
//};
//
//UCLASS()
//class NAVMESHOVERRIDE_API AGraph: public AActor
//{
//	GENERATED_BODY()
//
//public:
//	AGraph();
//	TArray<FGNode*> GetRelations() { return Nodes; }
//	void AddRelation(AActor* Edge, AActor* Connection);
//	void AddRelation(AActor* Edge, TArray<AActor*> Connections);
//	void VisitEdge(AActor* Edge);
//	bool IsEdgeInGraph(AActor* Edge);
//	FGNode* GetNodeByEdge(AActor* Edge);
//
//	static TArray<AActor*> DFS(AGraph G, AActor* Start);
//	static TArray<AActor*> BFS(AGraph G, AActor* Start);
//private:
//	TArray<FGNode*> Nodes;
//};
//
