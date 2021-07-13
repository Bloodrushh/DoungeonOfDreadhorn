// Fill out your copyright notice in the Description page of Project Settings.


#include "FightManager.h"
#include "DungeonManager.h"
#include "EventTriggerBase.h"

// Sets default values
AFightManager::AFightManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AFightManager::BeginPlay()
{
	Super::BeginPlay();	
}

void AFightManager::FormQueue()
{
	UE_LOG(LogTemp, Warning, TEXT("AFightManager::FormQueue called."));
	Participants.Sort([](AFightParticipant& First, AFightParticipant& Second)->bool
	{;
		UE_LOG(LogTemp, Warning, TEXT("First.GetInitiative(): %d; Second.GetInitiative(): %d"), First.GetInitiative(), Second.GetInitiative());
		return First.GetInitiative() > Second.GetInitiative();
	});

	for (auto Participant : Participants)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Participant->GetHumanReadableName());
	}
}

void AFightManager::FinishFight(bool bWin)
{
	EventTrigger->FinishEvent(bWin);
	
	if (!bWin)
	{
		// handle loose state. Teleport to table, regenerate dungeon
		DungeonManager->ResetDungeon();
	}
}

/*void AFightManager::SpawnEnemies(TMap<TSoftClassPtr<AFightParticipant>, int32> InEnemiesClasses)
{
	
}*/

void AFightManager::BeginFight_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("AFightManager::BeginFight_Implementation called"));
	Participants[0]->PerformMove();
}

void AFightManager::BeginFight_Delayed()
{
	UE_LOG(LogTemp, Warning, TEXT("AFightManager::BeginFight_Delayed called"));
	GetWorldTimerManager().SetTimer(BeginFightHandle, this, &AFightManager::BeginFight, BeginFightDelay, false);
}

void AFightManager::AddParticipant(AFightParticipant* Participant)
{
	Participants.Add(Participant);
}

void AFightManager::RemoveParticipant(AFightParticipant* Participant)
{
	Participants.Remove(Participant);
}

void AFightManager::OnMovePerformed(AFightParticipant* Initiator)
{
	UE_LOG(LogTemp, Warning, TEXT("AFightManager::OnMovePerformed called"));

	// Move first to the end and move queue
	AFightParticipant* ActiveParticipant = Participants[0];
	Participants.RemoveAt(0);
	Participants.Add(ActiveParticipant);	

	// we need to check that did enemies die
}

void AFightManager::InitiateFight(FFightInitiateInfo Info)
{
	// we need to spawn enemies before form queue
	SpawnEnemies(Info);
	Participants.Add(PlayerPawn);
	FormQueue();
	BeginFight_Delayed();
}

void AFightManager::OnParticipantDied(AFightParticipant* InParticipant)
{
	// need to think about how to give fight manager info about players death
	if(InParticipant == PlayerPawn)
	{
		FinishFight(false);
	}
	else
	{
		Participants.Remove(InParticipant);
		
		if (Enemies.Num() <= 0)
		{
			FinishFight(true);
		}
	}
}

void AFightManager::ResetFight()
{
	for (auto Enemy : Enemies)
	{
		Enemy->Destroy();
	}

	Enemies.Empty();
	Participants.Empty();
}

