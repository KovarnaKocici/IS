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
#include "Math/NumericLimits.h"
#include "Graph.h"

#pragma optimize("", off)


void APackmanAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Targets.Num())
	{
		SetNewMoveDestination(Targets[0]->GetActorLocation());

	}

	// Apply change in rotation 
	FVector velocity = (GetPawn()->GetVelocity());
	velocity.Normalize();
	FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(GetPawn()->GetActorLocation(), GetPawn()->GetActorLocation()+velocity);
	TargetRotator.Pitch = 0.f;
	TargetRotator.Roll = 0.f;
	GetPawn()->SetActorRotation(TargetRotator);

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

void APackmanAIController::RunBFS()
{
	ANavMeshOverrideGameMode* GM = Cast<ANavMeshOverrideGameMode>(GetWorld()->GetAuthGameMode());
	if (!GM || !GM->CoinsToCollect.Num())
	{
		return;
	}

	UGraph* G = GM->GenerateGraphFromLevel();
	Targets = UGraph::BFS(G, GetPawn(), GM->GetTarget());
}

void APackmanAIController::BeginPlay()
{
	Super::BeginPlay();
}
