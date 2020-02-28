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
	
	UPROPERTY()
	TArray<class ACoin*> CoinsToCollect;

	class UGraph* GenerateGraphFromLevel();

	AActor* GetTarget();

protected:
	virtual void BeginPlay() override;

	UPROPERTY( BlueprintReadWrite, /*, meta = (UIMin = '0.0', UIMax = '1.0'),*/ Category = "Graph")
	float Density = 0.5f;

};



