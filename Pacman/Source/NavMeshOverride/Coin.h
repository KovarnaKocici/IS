// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

UCLASS()
class NAVMESHOVERRIDE_API ACoin : public AActor
{
	GENERATED_BODY()
	
private:
	UMaterialInterface* TempMaterial;
	bool bWasTarget;
public:	
	// Sets default values for this actor's properties
	ACoin();

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UMaterialInterface* TargetMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graph")
	bool bIsTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void OnConstruction(const FTransform& Transform) override;


};
