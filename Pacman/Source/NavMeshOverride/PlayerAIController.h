// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Coin.h"
#include "PlayerAIController.generated.h"

UENUM()
enum class EPathfinderEnum : uint8
{
	PE_BFS UMETA(DisplayName = "BFS"),
	PE_DFS UMETA(DisplayName = "DFS"),
	PE_Dijkstra UMETA(DisplayName = "Dijkstra"),
	PE_AStar UMETA(DisplayName = "AStar")
};

UCLASS()
class NAVMESHOVERRIDE_API APlayerAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> Targets;

	virtual void Tick(float DeltaTime) override;

	virtual void Possess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	void SetNewMoveDestination(const FVector DestLocation);

	UFUNCTION(BlueprintCallable)
	void RunPathfinder(EPathfinderEnum Method);

private:
	TArray<float> Path;

	void RunDijkstra();
	void RunBFS();
	void RunDFS();

protected:
	virtual void BeginPlay() override;

};
