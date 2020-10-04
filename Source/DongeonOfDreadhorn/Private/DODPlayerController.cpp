// Fill out your copyright notice in the Description page of Project Settings.


#include "DODPlayerController.h"
#include "TimerManager.h"

void ADODPlayerController::SetBoardPawnReference(ABoardPawn* BoardPawnRef)
{
	BoardPawn = BoardPawnRef;
}

void ADODPlayerController::SetPlayerPawnReference(APlayerPawn * PlayerPawnRef)
{
	PlayerPawn = PlayerPawnRef;
}

void ADODPlayerController::TravelToBoard()
{
	if (BoardPawn && GetPawn() != BoardPawn)
	{
		PendingPawnToPosses = BoardPawn;
		PlayerCameraManager->StartCameraFade(0, 1, CameraFadeTime, FLinearColor::Black, false, true);
		PossessPendingPawn_Delayed();
	}
}

void ADODPlayerController::TravelToDungeon()
{
	if (PlayerPawn && GetPawn() != PlayerPawn)
	{
		PendingPawnToPosses = PlayerPawn;
		PlayerCameraManager->StartCameraFade(0, 1, CameraFadeTime, FLinearColor::Black, false, true);
		PossessPendingPawn_Delayed();
	}
}

void ADODPlayerController::PossessPendingPawn_Delayed()
{
	GetWorldTimerManager().SetTimer(PossessPendingPawnHandle, this, &ADODPlayerController::PossessPendingPawn, PossessDelay, false);
}

void ADODPlayerController::PossessPendingPawn()
{
	GetWorldTimerManager().ClearTimer(PossessPendingPawnHandle);
	if (PendingPawnToPosses)
	{
		if (PendingPawnToPosses == PlayerPawn)
		{
			PlayerPawn->DisableControllers();
			BoardPawn->EnableControllers();
		}
		else
		{
			BoardPawn->DisableControllers();
			PlayerPawn->EnableControllers();
		}
		Possess(PendingPawnToPosses);
		PlayerCameraManager->StartCameraFade(1, 0, CameraFadeTime, FLinearColor::Black, false, false);
	}
}
