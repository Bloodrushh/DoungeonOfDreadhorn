// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TableBase.h"
#include "ProjectTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventTriggerBase.generated.h"


class ADoungeonChunkBase;

UCLASS()
class DONGEONOFDREADHORN_API AEventTriggerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventTriggerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FEventInfo EventInfo;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		TArray<USceneComponent*> EnemySpawnPoints;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		TArray<USceneComponent*> ChestSpawnPoints;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
		bool TargetIsOnLineOfSight(AActor* InTarget);

	void OnEventFinished();
	
	void OnEventStarted();

	UPROPERTY(BlueprintReadOnly)
		bool bIsTriggered = false;

	UFUNCTION(BlueprintCallable)
		void SetIsTriggered(bool bTriggered);
	
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		ADoungeonChunkBase* Chunk;

	UFUNCTION(BlueprintCallable)
		void OnTriggered(AActor* TriggeredActor);

	FOnEventProcessed OnEventProcessedDelegate;

	UFUNCTION()
	void OnEventProcessed(int32 DeterminedValue, APlayerPawn*& InPlayerPawn);


	UFUNCTION(BlueprintImplementableEvent)
		void OnEventProcessedBP();
	
		void FinishEvent(bool bSucceed);

	UFUNCTION(BlueprintImplementableEvent)
		void OnEventFinishedBP(bool bSucceed);
	
};
