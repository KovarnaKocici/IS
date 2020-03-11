// Fill out your copyright notice in the Description page of Project Settings.

#include "Coin.h"
#include "Materials/MaterialInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NavMeshOverrideCharacter.h"
#include "PlayerAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

#pragma optimize( "", off)

// Sets default values
ACoin::ACoin(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(CollisionComponent);

	//DefaultMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("DefaultMaterialComponent"));
	//TargetMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("TargetMaterialComponent"));

}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACoin::NotifyActorBeginOverlap(AActor* OtherActor)
{
}

void ACoin::OnConstruction(const FTransform& Transform)
{
	if (bIsTarget)
	{
		MeshComponent->SetMaterial(0, TargetMaterial);
	}
	else
	{
		MeshComponent->SetMaterial(0, DefaultMaterial);
	}

}
