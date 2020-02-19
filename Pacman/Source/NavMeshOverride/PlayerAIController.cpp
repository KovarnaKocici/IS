// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "AI/Navigation/NavigationSystem.h"
#include "LogMacros.h"
#include "NavMeshOverrideGameMode.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"

#pragma optimize("", off)

void APackmanAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Targets && Targets->Num())
	{
		SetNewMoveDestination((*Targets)[0]->GetActorLocation());

	}

	// Apply change in rotation 
	FVector velocity = (GetPawn()->GetVelocity());
	velocity.Normalize();
	FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), GetPawn()->GetActorLocation()+velocity);
	TargetRotator.Pitch = 0.f;
	TargetRotator.Roll = 0.f;
	GetPawn()->SetActorRotation(TargetRotator);
	//FaceRotation(TargetRotator, 0.f);

}

void APackmanAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
}

void APackmanAIController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystem* const NavSys = GetWorld()->GetNavigationSystem();

		if (NavSys)
		{
			NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void APackmanAIController::BeginPlay()
{
	Super::BeginPlay();
	
	ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM || !GM->CoinsToCollect.Num())
	{
		return;
	}

	Targets = &(GM->CoinsToCollect);

	//Find optimal path
	if (Targets->Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Was found %i actors"), Targets->Num());

		//Initialize graph
		for (int i = 0; i < GM->CoinsToCollect.Num() + 1; i++)
		{
			TArray<float> Temp;
			Temp.Init(0.f, Targets->Num() + 1);
			CoinsGraph.Init(Temp, Targets->Num() + 1);

		}
		for (int i = 0; i < Targets->Num() + 1; i++)
		{
			for (int j = 0; j < Targets->Num() + 1; j++)
			{
				if (i == 0)
					i++;
				//Setup weights

			}
		}
	}
}
