// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BoardBase.h"
#include "CoreMinimal.h"
#include "DoungeonChunkBase.h"
#include "GameFramework/Actor.h"

#include "DungeonGeneratorBase.generated.h"

class ADungeonManager;
UCLASS()
class DONGEONOFDREADHORN_API ADungeonGeneratorBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADungeonGeneratorBase();		

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABoardBase* Board;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerPawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ADungeonManager* DungeonManager;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnChunk();
	
	void PlaceDeadEnds();

	void PlaceEventTriggers();

	void PlaceSecretRooms();
	
public:
	UFUNCTION(BlueprintCallable)
	void Reset();

protected:
	UPROPERTY()
	TArray<TSubclassOf<ADoungeonChunkBase>> BannedChunkClasses;

	UPROPERTY()
	ADoungeonChunkBase* LastSpawnedChunk;

	UPROPERTY()
	TArray<ADoungeonChunkBase*> SpawnedChunks;

	UPROPERTY()
	int32 ChunksAmount;
};
