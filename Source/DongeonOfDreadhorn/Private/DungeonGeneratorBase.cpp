// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGeneratorBase.h"

#include "DoungeonConfigurationSubsystem.h"
#include "Engine/World.h"

// Sets default values
ADungeonGeneratorBase::ADungeonGeneratorBase()
{
}

// Called when the game starts or when spawned
void ADungeonGeneratorBase::BeginPlay()
{
	Super::BeginPlay();

	//Setup();
	
	SpawnChunk();
}

void ADungeonGeneratorBase::SpawnChunk()
{
	TSubclassOf<ADoungeonChunkBase> ChunkClass;
	FTransform SpawnTansform;
	bool bFoundValidSpawnTransform = false;
	bool bLasChunk = false;	

	auto DungeonConfigurationSubsystem = GetGameInstance()->GetSubsystem<UDoungeonConfigurationSubsystem>();
	
	if (ChunksAmount == DungeonConfigurationSubsystem->Configuration.Length)
	{
		bLasChunk = true;
		ChunkClass = DungeonConfigurationSubsystem->Configuration.ExitChunkClass.LoadSynchronous();
	}

	if (LastSpawnedChunk == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("LastSpawnedChunk is NOT valid. Need to Spawn Entrance."));
		ChunkClass = DungeonConfigurationSubsystem->Configuration.EntranceChunkClass.LoadSynchronous();
		SpawnTansform.SetLocation(FVector::ZeroVector);
		bFoundValidSpawnTransform = true;		
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("LastSpawnedChunk is valid. Getting snap transform."));
		if (bLasChunk)
		{
			ChunkClass = DungeonConfigurationSubsystem->Configuration.ExitChunkClass.LoadSynchronous();
		}
		else
		{
			TArray<TSubclassOf<ADoungeonChunkBase>> ChunksPool;
			for (auto Chunk : DungeonConfigurationSubsystem->Configuration.ChunkClasses)
			{
				auto LoadedChunk = Chunk.LoadSynchronous();
				
				if (!BannedChunkClasses.Contains(LoadedChunk))
				{
					ChunksPool.Add(LoadedChunk);
				}
			}
			//ChunksPool.Remove(LastSpawnedChunk->GetClass());
			if (ChunksPool.Num() > 0)
			{
				ChunkClass = ChunksPool[FMath::RandRange(0, ChunksPool.Num() - 1)];
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ChunksPool.Num() < 0"));
			}
			
		}
		bFoundValidSpawnTransform = LastSpawnedChunk->TryGetSpawnTransformForChunk(ChunkClass, SpawnTansform);
	}

	FActorSpawnParameters SpawnParams;
	ADoungeonChunkBase* SpawnedChunk = nullptr;

	if (bFoundValidSpawnTransform)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Successfully got snap transform: x= %f, y= %f, z= %f. Trying to spawn chunk class: %s"), SpawnTansform.GetLocation().X, SpawnTansform.GetLocation().Y, SpawnTansform.GetLocation().Z, *ChunkClass.Get()->GetName());
		SpawnedChunk = GetWorld()->SpawnActor<ADoungeonChunkBase>(ChunkClass, SpawnTansform, SpawnParams);
		SpawnedChunk->SetDungeonManager(DungeonManager);
		SpawnedChunk->SetPlayerPawn(PlayerPawn);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Failed to get spawn transform. Trying to get spawn transform from previous chunk"));

		for (int32 i = SpawnedChunks.Num() - 1; i > 0; i--)
		{
			bFoundValidSpawnTransform = SpawnedChunks[i]->TryGetSpawnTransformForChunk(ChunkClass, SpawnTansform);
			if (bFoundValidSpawnTransform)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Successfully got snap transform from previous chunk: %s, transform: x=%f, y=%f, z=%f"), *SpawnedChunks[i]->GetHumanReadableName(), SpawnTansform.GetLocation().X, SpawnTansform.GetLocation().Y, SpawnTansform.GetLocation().Z);
				LastSpawnedChunk = SpawnedChunks[i];
				SpawnedChunk = GetWorld()->SpawnActor<ADoungeonChunkBase>(ChunkClass, SpawnTansform, SpawnParams);
				SpawnedChunk->SetDungeonManager(DungeonManager);
				SpawnedChunk->SetPlayerPawn(PlayerPawn);
				break;
			}
		};
		
	}

	if (SpawnedChunk != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Successfully spawned chunk: %s"), *SpawnedChunk->GetHumanReadableName());
		
		SpawnedChunks.Add(SpawnedChunk);
		BannedChunkClasses.Empty();
		BannedChunkClasses.Add(ChunkClass);
		LastSpawnedChunk = SpawnedChunk;
		
		if (bLasChunk)
		{
			UE_LOG(LogTemp, Warning, TEXT("Exit chunk spawned. Placing DeadEnds."));

			PlaceSecretRooms();
			PlaceDeadEnds();
			PlaceEventTriggers();
			if (Board)
			{
				Board->OnDungeonGenerated(SpawnedChunks);
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Is NOT last chunk. Spawning next. Current ChunksAmount: %d"), ChunksAmount);
			ChunksAmount++;
			SpawnChunk();
		}
	}
	else
	{
		// Change chunk class and retry 
		BannedChunkClasses.Add(ChunkClass);
		if (BannedChunkClasses.Num() < DungeonConfigurationSubsystem->Configuration.ChunkClasses.Num())
		{
			SpawnChunk();
		}		
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn chunk class: %s"), *ChunkClass.Get()->GetName());
	}	
}

void ADungeonGeneratorBase::PlaceDeadEnds()
{
	for (auto Chunk : SpawnedChunks)
	{
		Chunk->PlaceDeadEnds();
	}
}

void ADungeonGeneratorBase::PlaceEventTriggers()
{	
	auto DungeonConfigurationSubsystem = GetGameInstance()->GetSubsystem<UDoungeonConfigurationSubsystem>();	
	int32 ChunkIndex = 0;

	//-1 to exclude exit chunk
	int32 Step = (DungeonConfigurationSubsystem->Configuration.Length - 1) / DungeonConfigurationSubsystem->Configuration.EventTriggersSpawnAmount;	
	
	for (int32 i = 0; i < DungeonConfigurationSubsystem->Configuration.EventTriggersSpawnAmount; i++)
	{
		ChunkIndex += Step;
		SpawnedChunks[ChunkIndex]->SpawnEventTrigger();
	}
}

void ADungeonGeneratorBase::PlaceSecretRooms()
{
	/*int32 EventTriggersAmount = 0;
	int32 IterationsCount = 0;
	TArray <ADoungeonChunkBase*> ValidChunks = SpawnedChunks;
	ValidChunks.RemoveAt(0);
	ValidChunks.RemoveAt(ValidChunks.Num() - 1);

	while (EventTriggersAmount < EventTriggersToSpawn && IterationsCount < SpawnedChunks.Num() - 3)
	{
		int32 ChunkIndex = FMath::RandRange(0, ValidChunks.Num() - 1);
		if (ValidChunks[ChunkIndex]->GetCanSpawnEventTriggers())
		{
			if (ValidChunks[ChunkIndex]->SpawnEventTrigger())
			{
				EventTriggersAmount++;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Chunk: %s FAILED to spawn event triggers"), *ValidChunks[ChunkIndex]->GetHumanReadableName());
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Chunk: %s is not able to spawn event triggers"), *ValidChunks[ChunkIndex]->GetHumanReadableName());
		}
		ValidChunks.RemoveAt(ChunkIndex);
		IterationsCount++;
	}*/
}

void ADungeonGeneratorBase::Reset()
{
	for (auto Chunk :SpawnedChunks)
	{
		Chunk->Disappear();
	}
	
	SpawnedChunks.Empty();

	SpawnChunk();	
}



