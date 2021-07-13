// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "ProjectTypes.h"
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "DamageableInterface.h"
#include "FightParticipant.h"

#include "PlayerPawn.generated.h"

class AHandBase;
class AFootPrintDecal;

UCLASS()
class DONGEONOFDREADHORN_API APlayerPawn : public AFightParticipant
    , public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent)
	void Die();
	void Die_Implementation();

	FTransform CachedDungeonTransform;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) /*, meta = (AllowPrivateAccess = "true")*/
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FCharacterInfo> Characters;

	UPROPERTY()
	int32 ActiveCharacterIndex;

	UFUNCTION(BlueprintCallable)
	void AddCharacterToParty(FCharacterInfo InCharacter, int32& OutIndex);

	UFUNCTION(BlueprintCallable)
	bool RemoveCharacterFromParty(int32 InIndex);

	UFUNCTION(BlueprintCallable)
	void GetAttributeValue(EAttribute InAttribute, bool bGroup, int32& OutValue);

	UFUNCTION(BlueprintCallable)
	void ChangeAttributeValue(EEffect Effect, EAttribute Attribute, int32 inValue);

	void TakeDamage(int32 Amount, EAttack Attack) override;

	bool CanTakeDamage() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTakenBP(int32 Amount, EAttack Attack);

	virtual int32 GetInitiative() override;

	virtual void PerformMove() override;

	virtual void OnMovePerformed() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;

	void TeleportIntoFight(FVector Location);

	void TeleportIntoDungeon();

	UPROPERTY()
	bool bCanMove = true;

	UFUNCTION(BlueprintCallable)
	void ToggleCanMove(bool bCan);
};
