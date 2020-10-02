// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BoardPawnHand.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoardPawn.generated.h"

class UCameraComponent;

UCLASS()
class DONGEONOFDREADHORN_API ABoardPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABoardPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* VrOrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABoardPawnHand> HandClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AHandBase* LeftHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AHandBase* RightHand;
};
