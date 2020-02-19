// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NavMeshOverrideGameMode.h"
#include "NavMeshOverridePlayerController.h"
#include "NavMeshOverrideCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Coin.h"

#pragma optimize("", off)

ANavMeshOverrideGameMode::ANavMeshOverrideGameMode()
{
}

void ANavMeshOverrideGameMode::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<ACoin> classToFind;
	classToFind = ACoin::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, CoinsToCollect);
}
