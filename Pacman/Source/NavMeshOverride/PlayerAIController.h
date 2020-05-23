// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Coin.h"
#include "PlayerAIController.generated.h"

UENUM()
enum class EPathfinder : uint8
{
	PE_BFS UMETA(DisplayName = "BFS"),
	PE_DFS UMETA(DisplayName = "DFS"),
	PE_Dijkstra UMETA(DisplayName = "Dijkstra"),
	PE_AStar UMETA(DisplayName = "AStar"),
	PE_MinMax UMETA(DisplayName = "MinMax")
};

USTRUCT(BlueprintType)
struct FEstParams
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estimation")
	float Pacman = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estimation")
	float Ghost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estimation")
	float BaseCoin = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Estimation")
	float TragetCoin = 0;

};

UCLASS()
class NAVMESHOVERRIDE_API APlayerAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> Targets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathFinding")
	EPathfinder CurrentPathfinder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathFinding")
	FEstParams Params;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathFinding")
	int SearchDepth = 3;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathFinding")
	bool bIsReadyToRun = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathFinding")
	bool bEvaluate = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathFinding")
	bool bShouldEvaluateOnce = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PathFinding")
	bool bNewPathWasFound = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "PathFinding")
	int EvaluationTimes = 0;

	virtual void Tick(float DeltaTime) override;

	virtual void Possess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	void RunPathfinder(EPathfinder Method);

	float EvaluatePath(TArray<AActor*> Path);


private:
	TArray<float> Path;

	TArray<AActor*> GetBestPath(TArray<TArray<AActor*>> Paths);
	class AGraph* GetCurrentGraph();
	void PrintPathfinderSummary(EPathfinder Method, double StartTime, double EndTime);

protected:
	virtual void BeginPlay() override;

};
