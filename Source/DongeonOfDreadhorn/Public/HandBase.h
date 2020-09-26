// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MotionControllerComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandBase.generated.h"

class UMotionControllerComponent;
class USkeletalMeshComponent;

UCLASS()
class DONGEONOFDREADHORN_API AHandBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UMotionControllerComponent* MotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		EControllerHand Hand;

	virtual void OnConstruction(const FTransform& Transform) override;
};
