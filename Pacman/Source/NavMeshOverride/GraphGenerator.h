// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Graph.h"
#include "GraphGenerator.generated.h"

class AGraph;

UENUM()
enum class EGraphGenerator : uint8
{
	GGE_Default UMETA(DisplayName = "DefaultGenerator"),
	GGE_Maze UMETA(DisplayName = "MazeGenerator")
};

UCLASS()
class NAVMESHOVERRIDE_API AGraphGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGraphGenerator();

	UFUNCTION(BlueprintCallable/*, CallInEditor*/)
	bool CanGenerateGraph();

	UFUNCTION(BlueprintCallable/*, CallInEditor*/)
	void GenerateGraphFromLevel();

	UFUNCTION(BlueprintCallable/*, CallInEditor*/)
	void GenerateGraphMaze();

	UFUNCTION(BlueprintCallable)
	void AddRandomConnections();

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (UIMin = 0, UIMax = 200, ExposeOnSpawn = true), Category = "Generator")
	//int Size = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Generator")
	AGraph* Graph;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Generator")
	EGraphGenerator Type = EGraphGenerator::GGE_Default;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Generator"/*, meta = (UIMin = '0.0', UIMax = '1.0'),*/ )
	float GraphDensity = 0.2f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void RecursiveBackTracker(AActor* Node);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
