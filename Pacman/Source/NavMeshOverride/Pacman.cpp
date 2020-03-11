// Fill out your copyright notice in the Description page of Project Settings.

#include "Pacman.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavMeshOverrideGameMode.h"
#include "Coin.h"
#include "Kismet/KismetMathLibrary.h"

#pragma optimize ("", off)
// Sets default values
APacman::APacman()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Configure pawn movement
	GetMovementComponent()->bConstrainToPlane = true;
	GetMovementComponent()->bSnapToPlaneAtStart = true;

}

// Called when the game starts or when spawned
void APacman::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APacman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Apply change in rotation 

	if (bOrientRotationToMovement)
	{
		FVector velocity = (GetVelocity());
		velocity.Normalize();
		FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + velocity);
		TargetRotator.Pitch = 0.f;
		TargetRotator.Roll = 0.f;
		SetActorRotation(TargetRotator);
	}
}

// Called to bind functionality to input
void APacman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APacman::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (OtherActor->IsA(ACoin::StaticClass()))
	{
		ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			//GM->CoinsToCollect.Remove(Cast<ACoin>(OtherActor));
			//OtherActor->SetActorHiddenInGame(true);
		}
	}
}

