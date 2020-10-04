// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardBase.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABoardBase::ABoardBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoardBase::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ABoardBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABoardBase::OnDungeonGenerated(TArray<ADoungeonChunkBase*> Chunks)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDungeonGenerated Called"));

	if (DungeonChunkBoardClass)
	{
		UWorld* World = GetWorld();
		FVector DungeonChunkStartLocation;

		if (Chunks.IsValidIndex(0))
		{
			if (Chunks[0]->Floors.IsValidIndex(0))
			{
				DungeonChunkStartLocation = Chunks[0]->Floors[0]->GetComponentLocation();
			}
			else
			{
				return;
			}
			
		}
		else
		{
			return;
		}
		
		for (auto Chunk : Chunks)
		{
			for (auto Floor : Chunk->Floors)
			{
				FVector DeltaVector = (Floor->GetComponentLocation() - DungeonChunkStartLocation) / 200;
				FVector Location = GetActorLocation() + DeltaVector;
				FRotator Rotation = Floor->GetComponentRotation();
				FTransform SpawnTransform = FTransform(Rotation, Location);

				//FActorSpawnParameters SpawnParameters;
				//ADungeonChunkBoardBase* SpawnedChunk = World->SpawnActor<ADungeonChunkBoardBase>(DungeonChunkBoardClass, Location, Rotation, SpawnParameters);
				
				ADungeonChunkBoardBase* DeferredChunk = World->SpawnActorDeferred<ADungeonChunkBoardBase>(DungeonChunkBoardClass, SpawnTransform);
				DeferredChunk->bVisited = Chunk->bVisited;
				ADungeonChunkBoardBase* SpawnedChunk = Cast<ADungeonChunkBoardBase>(UGameplayStatics::FinishSpawningActor(DeferredChunk, SpawnTransform));
				if (SpawnedChunk)
				{
					BoardChunks.Add(SpawnedChunk);
					FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
					SpawnedChunk->AttachToActor(this, AttachmentRules);
					Chunk->AddBoardChunk(SpawnedChunk);
				}				
			}				
			/*FTransform RelativeTransform;
			AddComponent(TEXT(""), false, RelativeTransform, this);*/
		}
		OnMapGenerated();
	}
}

void ABoardBase::OnMapGenerated()
{
	float MaxX;
	float MinX;
	float MaxY;
	float MinY;
	Table->GetBounce(MaxX, MinX, MaxY, MinY);
	//UE_LOG(LogTemp, Warning, TEXT("MaxX: %f, MinX: %f, MaxY: %f, MinY: %f"), MaxX, MinX, MaxY, MinY);

	float OffsetX = 0.0f;
	float OffsetY = 0.0f;
	//UE_LOG(LogTemp, Warning, TEXT("BoardChunks.NUM(): %d"), BoardChunks.Num());

	for (auto Chunk : BoardChunks)
	{
		FVector ChunkLocation = Chunk->GetActorLocation();
		//UE_LOG(LogTemp, Warning, TEXT("ChunkLocation: X=%f, Y=%f"), ChunkLocation.X, ChunkLocation.Y);

		float DeltaLocation;

		if (ChunkLocation.X > MaxX)
		{
			DeltaLocation = MaxX - ChunkLocation.X;
			if (FMath::Abs(DeltaLocation) > FMath::Abs(OffsetX))
			{
				OffsetX = DeltaLocation;
			}			
		}
		if (ChunkLocation.X < MinX)
		{
			DeltaLocation = MinX - ChunkLocation.X;
			if (FMath::Abs(DeltaLocation) > FMath::Abs(OffsetX))
			{
				OffsetX = DeltaLocation;
			}
		}
		if (ChunkLocation.Y > MaxY)
		{
			DeltaLocation = MaxY - ChunkLocation.Y;
			if (FMath::Abs(DeltaLocation) > FMath::Abs(OffsetY))
			{
				OffsetY = DeltaLocation;
			}
		}
		if (ChunkLocation.Y < MinY)
		{
			DeltaLocation = MinY - ChunkLocation.Y;
			if (FMath::Abs(DeltaLocation) > FMath::Abs(OffsetY))
			{
				OffsetY = DeltaLocation;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("OffsetX: %f, OffsetY: %f"), OffsetX, OffsetY);
	AddActorWorldOffset(FVector(OffsetX, OffsetY, 0));
}