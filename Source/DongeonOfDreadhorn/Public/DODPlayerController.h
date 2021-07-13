// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "DODPlayerController.generated.h"

class APlayerPawn;
class ABoardPawn;
/**
 * 
 */
UCLASS()
class DONGEONOFDREADHORN_API ADODPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	APawn* PendingPawn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TeleportDelay = 2.0f;

	FTimerHandle PossessPendingPawnHandle;

	FTimerHandle TeleportIntoFightHandle;

	FVector PendingTeleportLocation;

	UPROPERTY()
	APlayerPawn* PlayerPawn;

	UPROPERTY()
	ABoardPawn* BoardPawn;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn();

	UFUNCTION(BlueprintImplementableEvent)
	void FadeOut();

public:
	void SetupPawnReferences(APlayerPawn* InPlayerPawn, ABoardPawn* InBoardPawn);

	void MakeTransitionFromBoardToFight(FVector Location);

	void MakeTransitionFromBoardToDungeon();

	void MakeTransitionFromDungeonToBoard();

	void MakeTransitionFromFightToDungeon();
};
