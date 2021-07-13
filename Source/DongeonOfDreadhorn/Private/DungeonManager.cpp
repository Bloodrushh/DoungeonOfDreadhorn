// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonManager.h"
#include "DODPlayerController.h"
#include "FightManager.h"
#include "Kismet/GameplayStatics.h"
#include "FightManager.h"

// Sets default values
ADungeonManager::ADungeonManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ADungeonManager::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<ADODPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	PlayerController->SetupPawnReferences(PlayerPawn, BoardPawn);
}

void ADungeonManager::MovePlayerIntoFight()
{
	UE_LOG(LogTemp, Warning, TEXT("ADungeonManager::MovePlayerIntoFight called"));
	//PlayerController->TeleportIntoFight(FightManager->GetTeleportLocation());
	//PlayerController->PossesPawn(PlayerPawn);
	PlayerController->MakeTransitionFromBoardToFight(FightManager->GetTeleportLocation());
}

void ADungeonManager::MovePlayerIntoTable()
{
	UE_LOG(LogTemp, Warning, TEXT("ADungeonManager::MovePlayerIntoTable called"));
	//PlayerController->PossesPawn(BoardPawn);
	PlayerController->MakeTransitionFromDungeonToBoard();
}

void ADungeonManager::MovePlayerIntoDungeonFromTable()
{
	UE_LOG(LogTemp, Warning, TEXT("ADungeonManager::MovePlayerIntoDungeonFromTable called"));
	//PlayerController->PossesPawn(PlayerPawn);
	PlayerController->MakeTransitionFromBoardToDungeon();	
}

void ADungeonManager::MovePlayerIntoDungeonFromFight()
{
	UE_LOG(LogTemp, Warning, TEXT("ADungeonManager::MovePlayerIntoDungeonFromFight called"));
	//PlayerController->PossesPawn(PlayerPawn);
	PlayerController->MakeTransitionFromFightToDungeon();
}

void ADungeonManager::ResetDungeon()
{
	DungeonGenerator->Reset();
}

void ADungeonManager::InitiateFightEvent()
{
	MovePlayerIntoFight();
	//FightManager->InitiateFight()
}

void ADungeonManager::InitiateTrapEvent()
{
	MovePlayerIntoTable();
}
