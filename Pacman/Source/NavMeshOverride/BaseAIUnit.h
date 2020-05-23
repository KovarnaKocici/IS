// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "BaseAIUnit.generated.h"

UCLASS()
class NAVMESHOVERRIDE_API ABaseAIUnit : public ADefaultPawn
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
	bool bOrientRotationToMovement = false;

	// Sets default values for this pawn's properties
	ABaseAIUnit(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool bShouldInfluenceTargets = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	bool bShouldRotateToTarget = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
};
