// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerFight.h"

void AEventTriggerFight::OnEventProcessed(int32 DeterminedValue, APlayerPawn*& InPlayerPawn)
{
	Super::OnEventProcessed(DeterminedValue, InPlayerPawn);
}

void AEventTriggerFight::OnEnemyDied_Implementation(AEnemyBase* Enemy)
{
	if(SpawnedEnemies.Contains(Enemy))
	{
		SpawnedEnemies.Remove(Enemy);
	}
	if(!(SpawnedEnemies.Num() > 0))
	{
		FinishEvent();
	}
}
