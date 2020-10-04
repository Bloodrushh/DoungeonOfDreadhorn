// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "HandBase.h"
#include "BoardPawnHand.generated.h"

/**
 * 
 */
UCLASS()
class DONGEONOFDREADHORN_API ABoardPawnHand : public AHandBase
{
	GENERATED_BODY()

public:

	ABoardPawnHand();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* GrabSphere;

	virtual void Tick(float DeltaTime) override;

	void Grab();

	bool GetNearestActor(AActor*& OutActor);

	void Drop();
	
	UPROPERTY()
		AActor* AttachedActor;
};
