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
	TArray<AActor*> CoinsToCollect;
protected:
	virtual void BeginPlay() override;

};



