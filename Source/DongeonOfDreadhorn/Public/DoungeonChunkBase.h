// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "DungeonChunkBoardBase.h"
#include "GameFramework/Actor.h"

#include "DoungeonChunkBase.generated.h"

class APlayerPawn;
class ADungeonManager;
class AEventTriggerBase;

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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADoungeonChunkBase> DeadEndChunkClass;	

	UPROPERTY(EditDefaultsOnly)
	FName ExitSnapPointTag = TEXT("ExitSnapPoint");

	UPROPERTY(EditDefaultsOnly)
	FName FloorTag = TEXT("FloorSnapPoint");		

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UStaticMeshComponent*> Floors;	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ADungeonChunkBoardBase*> BoardChunks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bVisited = false;	

	UPROPERTY(EditDefaultsOnly)
	bool CanSpawnEventTriggers = false;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftClassPtr<AEventTriggerBase>> EventTriggerClasses;

	UPROPERTY(BlueprintReadOnly)
	TArray<AEventTriggerBase*> EventTriggers;		

	UPROPERTY(BlueprintReadOnly)
	APlayerPawn* PlayerPawn;

	UPROPERTY(BlueprintReadOnly)
	ADungeonManager* DungeonManager;

	UFUNCTION(BlueprintCallable)
	void SetPlayerPawn(APlayerPawn* InPlayerPawn);

	UFUNCTION(BlueprintCallable)
	void SetDungeonManager(ADungeonManager* InDungeonManager);

protected:
	void FindAndCacheFloors();	

	void GetDeadExits(TArray<USceneComponent*>& OutExits);

	void GetFloorsFromChunkClass(TSubclassOf<ADoungeonChunkBase> ChunkClass, TArray<USceneComponent*>& OutFloors);

	void UpdateValidExits();	

	void FindAndCachePossibleExits();

	void GetPossibleExits(TArray<USceneComponent*>& OutPossibleExits);

	bool GetCanSpawnEventTriggers();	
		
public:
	UFUNCTION(BlueprintNativeEvent)
	void Disappear();
	void Disappear_Implementation();

	void PlaceDeadEnds();

	bool TryGetSpawnTransformForChunk(TSubclassOf<ADoungeonChunkBase> SpawningChunkClass, FTransform& OutTransform);

	void AddBoardChunk(ADungeonChunkBoardBase* NewBoardChunk);

	bool SpawnEventTrigger();
	
	UFUNCTION(BlueprintCallable)
	void OnVisited();
};