// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Coin.h"
#include "PlayerAIController.generated.h"

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
	void RunBFS();

	UFUNCTION(BlueprintCallable)
	void RunDFS();

private:
	TArray<float> Path;

protected:
	virtual void BeginPlay() override;

};
