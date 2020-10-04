// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PlayerPawn.h"
#include "BoardPawn.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DODPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DONGEONOFDREADHORN_API ADODPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void SetBoardPawnReference(ABoardPawn* BoardPawnRef);

	void SetPlayerPawnReference(APlayerPawn* PlayerPawnRef);

	UPROPERTY()
		ABoardPawn* BoardPawn;

	UPROPERTY()
		APlayerPawn* PlayerPawn;

	UFUNCTION(BlueprintCallable)
	void TravelToBoard();

	UFUNCTION(BlueprintCallable)
	void TravelToDungeon();

	APawn* PendingPawnToPosses;

	void PossessPendingPawn_Delayed();

	FTimerHandle PossessPendingPawnHandle;

	void PossessPendingPawn();

	UPROPERTY(EditDefaultsOnly)
		float CameraFadeTime = 1.0f;

	UPROPERTY(EditDefaultsOnly)
		float PossessDelay = 2.0f;
	
};
