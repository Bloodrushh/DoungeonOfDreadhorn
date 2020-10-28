// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventTriggerBase.h"
#include "EventTriggerFight.generated.h"

/**
 * 
 */
UCLASS()
class DONGEONOFDREADHORN_API AEventTriggerFight : public AEventTriggerBase
{
	GENERATED_BODY()

public:
	virtual void OnEventProcessed(int32 DeterminedValue, APlayerPawn*& InPlayerPawn) override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnEnemyDied(AEnemyBase* Enemy);

	void OnEnemyDied_Implementation(AEnemyBase* Enemy);

	UFUNCTION(BlueprintImplementableEvent)
		void SpawnEnemies();

	UPROPERTY(BlueprintReadOnly)
		TArray<AEnemyBase*> SpawnedEnemies;	
};
