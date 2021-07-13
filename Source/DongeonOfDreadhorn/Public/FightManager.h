// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FightParticipant.h"
#include "PlayerPawn.h"
#include "GameFramework/Actor.h"
#include "FightManager.generated.h"

class ADungeonManager;
class AEventTriggerBase;
class AFightParticipant;
class IFightable;

UCLASS()
class DONGEONOFDREADHORN_API AFightManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFightManager();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	APlayerPawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ADungeonManager* DungeonManager;

	UPROPERTY(BlueprintReadOnly)
	TArray<AFightParticipant*> Participants;

	UPROPERTY(BlueprintReadOnly)
	TArray<AFightParticipant*> Enemies;

	UPROPERTY(BlueprintReadOnly)
	AEventTriggerBase* EventTrigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BeginFightDelay = 5.0f;
	
	FTimerHandle BeginFightHandle;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FormQueue();

	void FinishFight(bool bWin);
	
	UFUNCTION(BlueprintNativeEvent)
	void BeginFight();	
	void BeginFight_Implementation();
	void BeginFight_Delayed();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnEnemies(FFightInitiateInfo Info);

	UFUNCTION(BlueprintCallable)
	void AddParticipant(AFightParticipant* Participant);

	void RemoveParticipant(AFightParticipant* Participant);
	
public:	
	UFUNCTION(BlueprintCallable)
	void OnMovePerformed(AFightParticipant* Initiator);	

	UFUNCTION(BlueprintCallable)
	void InitiateFight(FFightInitiateInfo Info);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, BlueprintPure)
	FVector GetTeleportLocation();

	UFUNCTION(BlueprintCallable)
	void OnParticipantDied(AFightParticipant* InParticipant);

	void ResetFight();
};


