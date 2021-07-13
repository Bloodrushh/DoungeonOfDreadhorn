// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BoardPawn.h"
#include "PlayerPawn.h"
#include "CoreMinimal.h"

#include "DungeonGeneratorBase.h"
#include "GameFramework/Actor.h"
#include "DungeonManager.generated.h"

class AFightManager;
class ADODPlayerController;

UCLASS()
class DONGEONOFDREADHORN_API ADungeonManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AFightManager* FightManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerPawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABoardPawn* BoardPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADungeonGeneratorBase* DungeonGenerator;

protected:
	UPROPERTY()
	ADODPlayerController* PlayerController;

public:	
	UFUNCTION(BlueprintCallable)
	void InitiateFightEvent();

	UFUNCTION(BlueprintCallable)
	void InitiateTrapEvent();

	UFUNCTION(BlueprintCallable)
	void MovePlayerIntoFight();

	UFUNCTION(BlueprintCallable)
	void MovePlayerIntoTable();

	UFUNCTION(BlueprintCallable)
	void MovePlayerIntoDungeonFromTable();

	UFUNCTION(BlueprintCallable)
	void MovePlayerIntoDungeonFromFight();

	UFUNCTION(BlueprintCallable)
	void ResetDungeon();
	
protected:
	virtual void BeginPlay() override;
	
	
};
