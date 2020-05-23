// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerAIController.h"
#include "BaseAIUnit.h"
#include "Ghost.generated.h"

/**
 * 
 */
UCLASS()
class NAVMESHOVERRIDE_API AGhost : public ABaseAIUnit
{
	GENERATED_BODY()
	
	public:
	AGhost(const FObjectInitializer& ObjectInitializer);
	
};
