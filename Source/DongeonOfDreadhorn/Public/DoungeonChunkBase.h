// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EventTriggerBase.h"
#include "DungeonChunkBoardBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoungeonChunkBase.generated.h"

UCLASS()
class DONGEONOFDREADHORN_API ADoungeonChunkBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoungeonChunkBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY()
		TArray<USceneComponent*> PossibleExits;

	UPROPERTY()
		TArray<USceneComponent*> ValidExits;

	UPROPERTY()
		TArray<USceneComponent*> EnemySpawnPoints;

	UPROPERTY()
		TArray<USceneComponent*> ChestSpawnPoints;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADoungeonChunkBase> DeadEndChunkClass;
	
		void UpdateValidExits();

	UFUNCTION(BlueprintCallable)
		bool TryGetSpawnTransformForChunk(TSubclassOf<ADoungeonChunkBase> SpawningChunkClass, FTransform & OutTransform);

		void FindAndChachePossibleExits();

	UPROPERTY(EditDefaultsOnly)
		FName ExitSnapPointTag = TEXT("ExitSnapPoint");

	UPROPERTY(EditDefaultsOnly)
		FName FloorTag = TEXT("FloorSnapPoint");

	UPROPERTY(EditDefaultsOnly)
		FName ChestSpawnPointTag = TEXT("ChestSpawnPoint");

	UPROPERTY(EditDefaultsOnly)
		FName EnemySpawnPointTag = TEXT("EnemySpawnPoint");

	UFUNCTION(BlueprintCallable)
		void GetPossibleExits(TArray<USceneComponent*>& OutPossibleExits);

	void PlaceDeadEnds();

	void GetDeadExits(TArray<USceneComponent*>& OutExits);

	void GetFloorsFromChunkClass(TSubclassOf<ADoungeonChunkBase> ChunkClass, TArray<USceneComponent*>& OutFloors);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<UStaticMeshComponent*> Floors;

	UFUNCTION(BlueprintCallable)
		void OnVisited();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<ADungeonChunkBoardBase*> BoardChunks;

	void AddBoardChunk(ADungeonChunkBoardBase* NewBoardChunk);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bVisited = false;

	void FindAndCacheEnemySpawnPoints();

	void FindAndCacheChestSpawnPoints();

	void FindAndCacheFloors();

	UPROPERTY(EditDefaultsOnly)
		bool CanSpawnEventTriggers = false;

	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<AEventTriggerBase>> EventTriggers;

	bool GetCanSpawnEventTriggers();

	bool SpawnEventTrigger();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADoungeonChunkBase> ExitBlockerClass;
	
	TArray<ADoungeonChunkBase*> ExitBlockers;

	void CloseExits();
	
	void OpenExits();
};
