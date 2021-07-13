// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectTypes.h"
#include "TableBase.h"

#include "EventTriggerBase.generated.h"

class ADungeonManager;
class AEnemyBase;
class ADoungeonChunkBase;

UCLASS()
class DONGEONOFDREADHORN_API AEventTriggerBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventTriggerBase();

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	TArray<USceneComponent*> ChestSpawnPoints;

	UPROPERTY(BlueprintReadOnly)
	bool bIsTriggered = false;

	FOnEventProcessed OnEventProcessedDelegate;

	UPROPERTY(BlueprintReadOnly)
	bool bEventResult = false;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	APlayerPawn* PlayerPawn;

	UPROPERTY(BlueprintReadOnly,  meta = (ExposeOnSpawn = true))
	ADungeonManager* DungeonManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FEventInfo EventInfo;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	ADoungeonChunkBase* Chunk;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool TargetIsOnLineOfSight(AActor* InTarget);	
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;	

	virtual void OnConstruction(const FTransform& Transform) override;		

	UFUNCTION(BlueprintCallable)
	void SetIsTriggered(bool bTriggered);	

	UFUNCTION(BlueprintNativeEvent)
	void OnTriggered(AActor* TriggeredActor);	
	void OnTriggered_Implementation(AActor* TriggeredActor);

	UFUNCTION()
	void OnEventProcessed(int32 DeterminedValue);
	UFUNCTION(BlueprintImplementableEvent)
	void OnEventProcessedBP(bool Result, int32 DeterminedValue);	

	UFUNCTION(BlueprintNativeEvent)
	void FinishEvent(bool Succeed);
	void FinishEvent_Implementation(bool Succeed);

	UFUNCTION(BlueprintNativeEvent)
	void Disappear();
	void Disappear_Implementation();
;	
};


