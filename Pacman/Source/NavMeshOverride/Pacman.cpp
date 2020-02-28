// Fill out your copyright notice in the Description page of Project Settings.

#include "Pacman.h"
#include "GameFramework/PawnMovementComponent.h"
#include "NavMeshOverrideGameMode.h"
#include "Coin.h"

#pragma optimize ("", off)
// Sets default values
APacman::APacman()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Configure pawn movement
	GetMovementComponent()->bConstrainToPlane = true;
	GetMovementComponent()->bSnapToPlaneAtStart = true;

	//GetMovementComponent()->bOrientRotationToMovement = true;
	//GetMovementComponent()->RotationRate = FRotator(0.f, 640.f, 0.f);
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
	GetMovementComponent()->GetActorFeetLocation();
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
			GM->CoinsToCollect.Remove(Cast<ACoin>(OtherActor));
			OtherActor->Destroy();
			OtherActor = nullptr;
		}
	}
}

