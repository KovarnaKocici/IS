// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Coin.h"
#include "PlayerAIController.generated.h"

UCLASS()
class NAVMESHOVERRIDE_API APackmanAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void Possess(APawn* InPawn) override;

	void SetNewMoveDestination(const FVector DestLocation);

	UFUNCTION(BlueprintCallable)
	void RunBFS();

private:
	TArray<float> Path;
	TArray<AActor*> Targets;
protected:
	virtual void BeginPlay() override;

};
