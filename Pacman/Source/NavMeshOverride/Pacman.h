// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerAIController.h"
#include "BaseAIUnit.h"
#include "Pacman.generated.h"

/**
 * 
 */
UCLASS()
class NAVMESHOVERRIDE_API APacman : public ABaseAIUnit
{
	GENERATED_BODY()
	
	public:
	APacman(const FObjectInitializer& ObjectInitializer);

};
