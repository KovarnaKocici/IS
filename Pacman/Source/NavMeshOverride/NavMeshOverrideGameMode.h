// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NavMeshOverrideGameMode.generated.h"


UCLASS(minimalapi)
class ANavMeshOverrideGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANavMeshOverrideGameMode();
	
	UFUNCTION(BlueprintCallable)
	void GenerateGraph();

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetTargets();

	UFUNCTION(BlueprintCallable)
	AActor* GetCoin(AActor* OnActor);

	//Including OnActor itself
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetAllUnits(AActor* OnActor);

	UPROPERTY()
	TArray<class ACoin*> CoinsToCollect;

	UPROPERTY()
	class AGraphGenerator* GraphGenerator;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Score = 0;

protected:
	virtual void BeginPlay() override;

	//UPROPERTY(BlueprintReadWrite, /*, meta = (UIMin = '0.0', UIMax = '1.0'),*/ Category = "Graph")
	//float GraphDrawSpeed = 1.f;

};



