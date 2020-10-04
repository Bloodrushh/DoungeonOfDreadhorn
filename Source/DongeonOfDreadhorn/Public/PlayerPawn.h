// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class AHandBase;
class AFootPrintDecal;

UCLASS()
class DONGEONOFDREADHORN_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward();

	UFUNCTION()
	void MoveRight();

	UFUNCTION()
	void MoveLeft();

	UFUNCTION()
	void TurnRight();

	UFUNCTION()
	void TurnLeft();

	UFUNCTION()
		void MakeStep();

	UFUNCTION()
		void MakeTurn();

	UPROPERTY()
		USceneComponent* VrOrigin;

	UPROPERTY()
		FVector DesiredLocation;
	
	UPROPERTY()
		FRotator DesiredRotation;

	bool bPendingStep = false;

	bool bPendingTurn = true;

	UPROPERTY(EditDefaultsOnly)
		float StepDistance = 500.0f;

	UPROPERTY(EditDefaultsOnly)
		float StepInterpSpeed = 1500.0f;

	UPROPERTY()
		float StepReachDistance = 0.1f;

	UPROPERTY(EditDefaultsOnly)
		float TurnInterpSpeed = 270.0f;

	UPROPERTY()
		float TurnReachDistance = 0.99999f;

	UPROPERTY(BlueprintReadOnly)
		float TurnAngle = 90.0f;

	UFUNCTION()
		bool CanMoveTo(FVector Destination);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)/*, meta = (AllowPrivateAccess = "true")*/
		UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AHandBase> HandClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AHandBase* LeftHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AHandBase* RightHand;

	UFUNCTION()
		void SpawnFootPrint();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AFootPrintDecal> FootPrintDecalClass;

	UPROPERTY()
		TArray<AFootPrintDecal*> FootPrints;

	UPROPERTY(EditDefaultsOnly)
		int32 MaxFootPrints;

	void DisableControllers();
	void EnableControllers();
};
