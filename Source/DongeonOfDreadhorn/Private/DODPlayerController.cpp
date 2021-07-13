// Fill out your copyright notice in the Description page of Project Settings.


#include "DODPlayerController.h"
#include "TimerManager.h"
#include "PlayerPawn.h"
#include "BoardPawn.h"

void ADODPlayerController::SetupPawnReferences(APlayerPawn* InPlayerPawn, ABoardPawn* InBoardPawn)
{
	PlayerPawn = InPlayerPawn;
	BoardPawn = InBoardPawn;
}

void ADODPlayerController::MakeTransitionFromBoardToFight(FVector Location)
{
	FadeIn();
	GetWorldTimerManager().SetTimer(TeleportIntoFightHandle, FTimerDelegate::CreateLambda([this, Location]()
	{
		Possess(PlayerPawn);
		PlayerPawn->TeleportIntoFight(Location);
		FadeOut();
	}),TeleportDelay, false, TeleportDelay);
}

void ADODPlayerController::MakeTransitionFromBoardToDungeon()
{
	FadeIn();
	GetWorldTimerManager().SetTimer(TeleportIntoFightHandle, FTimerDelegate::CreateLambda([this]()
	{
		Possess(PlayerPawn);
		FadeOut();
	}),TeleportDelay, false, TeleportDelay);
}

void ADODPlayerController::MakeTransitionFromDungeonToBoard()
{
	FadeIn();
	GetWorldTimerManager().SetTimer(TeleportIntoFightHandle, FTimerDelegate::CreateLambda([this]()
	{
		Possess(BoardPawn);
		FadeOut();
	}),TeleportDelay, false, TeleportDelay);
}

void ADODPlayerController::MakeTransitionFromFightToDungeon()
{
	FadeIn();
	GetWorldTimerManager().SetTimer(TeleportIntoFightHandle, FTimerDelegate::CreateLambda([this]()
	{
		PlayerPawn->TeleportIntoDungeon();
		FadeOut();
	}),TeleportDelay, false, TeleportDelay);
}