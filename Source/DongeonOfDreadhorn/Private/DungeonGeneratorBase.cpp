// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGeneratorBase.h"
#include "Engine/World.h"

// Sets default values
ADungeonGeneratorBase::ADungeonGeneratorBase()
{
}

// Called when the game starts or when spawned
void ADungeonGeneratorBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnChunk();
}

void ADungeonGeneratorBase::SpawnChunk()
{
	TSubclassOf<ADoungeonChunkBase> ChunkClass;
	FTransform SpawnTansform;
	bool bFoundValidSpawnTransform = false;
	bool bLasChunk = false;	

	if (ChunksAmount == ChunksToSpawn)
	{
		bLasChunk = true;
		ChunkClass = ExitChunkClass;
	}

	if (LastSpawnedChunk == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("LastSpawnedChunk is NOT valid. Need to Spawn Entrance."));
		ChunkClass = EntranceChunkClass;
		SpawnTansform.SetLocation(FVector::ZeroVector);
		bFoundValidSpawnTransform = true;		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LastSpawnedChunk is valid. Getting snap transform."));
		if (bLasChunk)
		{
			ChunkClass = ExitChunkClass;
		}
		else
		{
			ChunkClass = ChunkClasses[FMath::RandRange(0, ChunkClasses.Num() - 1)];
		}
		//ChunkClass = bLasChunk ? ExitChunkClass : ChunkClasses[FMath::RandRange(0, ChunkClasses.Num() - 1)];
		//ChunkClass = ChunkClasses[FMath::RandRange(0, ChunkClasses.Num() - 1)];
		bFoundValidSpawnTransform = LastSpawnedChunk->TryGetValidExitSnapTransform(SpawnTansform);		
	}

	FActorSpawnParameters SpawnParams;
	ADoungeonChunkBase* SpawnedChunk = nullptr;
	/*FProperty* ExitSnapPointTagProperty = ADoungeonChunkBase::StaticClass()->FindPropertyByName(TEXT("ExitSnapPointTag"));
	//ExitSnapPointTagProperty->->Des*/

	if (bFoundValidSpawnTransform)
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully got snap transform: x= %f, y= %f, z= %f. Trying to spawn chunk class: %s"), SpawnTansform.GetLocation().X, SpawnTansform.GetLocation().Y, SpawnTansform.GetLocation().Z, *ChunkClass.Get()->GetName());
		SpawnedChunk = GetWorld()->SpawnActor<ADoungeonChunkBase>(ChunkClass, SpawnTansform, SpawnParams);		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get spawn transform. Trying to get spawn transform from previous chunk"));

		for (int32 i = SpawnedChunks.Num() - 1; i > 0; i--)
		{
			bFoundValidSpawnTransform = SpawnedChunks[i]->TryGetValidExitSnapTransform(SpawnTansform);
			if (bFoundValidSpawnTransform)
			{
				UE_LOG(LogTemp, Warning, TEXT("Successfully got snap transform from previous chunk: %s, transform: x=%f, y=%f, z=%f"), *SpawnedChunks[i]->GetHumanReadableName(), SpawnTansform.GetLocation().X, SpawnTansform.GetLocation().Y, SpawnTansform.GetLocation().Z);
				LastSpawnedChunk = SpawnedChunks[i];
				SpawnedChunk = GetWorld()->SpawnActor<ADoungeonChunkBase>(ChunkClass, SpawnTansform, SpawnParams);
				break;
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("Trying to spawn chunk class: %s"), *ChunkClass.Get()->GetName());
		
	}
	
	//LastSpawnedChunk = GetWorld()->SpawnActor<ADoungeonChunkBase>(ChunkClass, SpawnTansform, SpawnParams);
	if (SpawnedChunk != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully spawned chunk: %s"), *SpawnedChunk->GetHumanReadableName());
		LastSpawnedChunk = SpawnedChunk;
		SpawnedChunks.Add(SpawnedChunk);
		
		if (bLasChunk)
		{
			UE_LOG(LogTemp, Warning, TEXT("Exit chunk spawned. Placing DeadEnds."));

			for (auto Chunk : SpawnedChunks)
			{
				Chunk->PlaceDeadEnds();
			}
			return;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Is NOT last chunk. Spawning next. Current ChunksAmount: %d"), ChunksAmount);
			ChunksAmount++;
			SpawnChunk();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn chunk class: %s"), *ChunkClass.Get()->GetName());
	}	
}



