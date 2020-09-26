// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DoungeonChunkBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonGeneratorBase.generated.h"

UCLASS()
class DONGEONOFDREADHORN_API ADungeonGeneratorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGeneratorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<ADoungeonChunkBase>> ChunkClasses;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADoungeonChunkBase> EntranceChunkClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADoungeonChunkBase> ExitChunkClass;

	UPROPERTY()
		TArray<TSubclassOf<ADoungeonChunkBase>> BannedChunkClasses;

	UPROPERTY()
		ADoungeonChunkBase* LastSpawnedChunk;

	UPROPERTY()
		TArray<ADoungeonChunkBase*> SpawnedChunks;

	UFUNCTION()
		void SpawnChunk();

	UPROPERTY(EditDefaultsOnly)
		int32 ChunksToSpawn = 30;

	int32 ChunksAmount;
};
