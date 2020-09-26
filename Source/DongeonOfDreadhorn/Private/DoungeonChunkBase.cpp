// Fill out your copyright notice in the Description page of Project Settings.


#include "DoungeonChunkBase.h"
#include "DrawDebugHelpers.h"

// Sets default values
ADoungeonChunkBase::ADoungeonChunkBase()
{

}

// Called when the game starts or when spawned
void ADoungeonChunkBase::BeginPlay()
{
	Super::BeginPlay();
	
	FindAndChachePossibleExits();
	ChooseValidExit();
	FindAndChacheFloors();
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
}

void ADoungeonChunkBase::ChooseValidExit()
{
	/*if (PossibleExits.Num() > 1)
	{
		ValidExitIndex = FMath::RandRange(0, PossibleExits.Num());
	}
	else
	{	
		
	}*/

	//TArray<USceneComponent*> FilteredExits;
	for (auto Exit : PossibleExits)
	{
		FVector TraceStart = Exit->GetComponentTransform().GetLocation();
		float TraceLength = (GetRootComponent()->GetComponentTransform().GetLocation() - Exit->GetComponentTransform().GetLocation()).Size2D();
		FVector TraceEnd = TraceStart + (Exit->GetForwardVector() * TraceLength);
		ECollisionChannel TraceChannel = ECollisionChannel::ECC_Visibility;
		FCollisionQueryParams QuerryParams;
		QuerryParams.AddIgnoredActor(this);
		/*FCollisionResponseParams ResponseParams;
		ResponseParams.CollisionResponse.Visibility;*/
		if (!GetWorld()->LineTraceTestByChannel(TraceStart, TraceEnd, TraceChannel, QuerryParams))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Exit: %s is NOT blocked."), *Exit->GetReadableName());
			//FilteredExits.Add(Exit);
			if (!ValidExits.Contains(Exit))
			{
				ValidExits.Add(Exit);
			}			
		}
		else
		{
			if (ValidExits.Contains(Exit))
			{
				ValidExits.Remove(Exit);
			}
			//UE_LOG(LogTemp, Warning, TEXT("Exit: %s is blocked"), *Exit->GetReadableName());
		}
	}

	if (ValidExits.Num() > 0)
	{
		ValidExitIndex = FMath::RandRange(0, ValidExits.Num() - 1);
		//UE_LOG(LogTemp, Warning, TEXT("ValidExitIndex: %d"), ValidExitIndex);
	}
	else
	{
		ValidExitIndex = -1;
	}
}

bool ADoungeonChunkBase::TryGetValidExitSnapTransform(FTransform & OutTransform)
{
	/*UWorld* World = GetWorld();
	TSubclassOf<ADoungeonChunkBase> ChunkClass;
	ADoungeonChunkBase* ChunkCDO = ChunkClass.GetDefaultObject();
	for (auto Floor : ChunkCDO->Floors)
	{
		FVector Start = Floor->GetComponentLocation() + FVector(0.0f, 0.0f, 250);
		FVector End = Floor->GetComponentLocation();
		FHitResult HitResult;
		bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
		if (bHit)
		{
			return false;
		}
	}*/


	ChooseValidExit();
	if (!ValidExits.IsValidIndex(ValidExitIndex)) { return false; }
	OutTransform = ValidExits[ValidExitIndex]->GetComponentTransform();
	return true;
}

void ADoungeonChunkBase::FindAndChachePossibleExits()
{
	for (auto ChildActoorComponent : GetComponentsByTag(USceneComponent::StaticClass(), ExitSnapPointTag))
	{
		USceneComponent* ChildSceneComponent = nullptr;
		ChildSceneComponent = Cast<USceneComponent>(ChildActoorComponent);
		if (ChildSceneComponent)
		{
			PossibleExits.Add(ChildSceneComponent);
		}		
	}
}

void ADoungeonChunkBase::FindAndChacheFloors()
{
	for (auto ChildComponent : GetComponentsByTag(USceneComponent::StaticClass(), FloorTag))
	{
		USceneComponent* SceneComponent = Cast<USceneComponent>(ChildComponent);
		if (SceneComponent)
		{
			Floors.Add(SceneComponent);
		}		
	}
}

void ADoungeonChunkBase::GetPossibleExits(TArray<USceneComponent*>& OutPossibleExits)
{
	OutPossibleExits = PossibleExits;
}

void ADoungeonChunkBase::PlaceDeadEnds()
{	
	//UE_LOG(LogTemp, Warning, TEXT("PacingDeadEnds for chunk: %s"), *GetHumanReadableName());
	//ChooseValidExit();

	TArray<USceneComponent*> DeadExits;
	GetDeadExits(DeadExits);
	FActorSpawnParameters SpawnParams;
	for (auto Exit : DeadExits)
	{	
		FTransform  SpawnTansform = Exit->GetComponentTransform();
		//UE_LOG(LogTemp, Warning, TEXT("Successfully found place for DeadEnd: x=%f, y=%f, z=%f"), SpawnTansform.GetLocation().X, SpawnTansform.GetLocation().Y, SpawnTansform.GetLocation().Z);
		GetWorld()->SpawnActor<ADoungeonChunkBase>(DeadEndChunkClass, SpawnTansform, SpawnParams);		
	}
}

void ADoungeonChunkBase::GetDeadExits(TArray<USceneComponent*>& OutExits)
{
	for (auto Exit : PossibleExits)
	{
		float TraceLength = ((GetRootComponent()->GetComponentTransform().GetLocation() - Exit->GetComponentTransform().GetLocation()).Size2D() / 2) - 50;
		FVector TraceStart = Exit->GetComponentTransform().GetLocation();		
		FVector TraceEnd = Exit->GetComponentTransform().GetLocation() + ((Exit->GetForwardVector()* -1) * TraceLength);

		ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldStatic;
		FCollisionQueryParams QuerryParams;
		FCollisionObjectQueryParams ObjectQuerryParams;
		FHitResult HitResult;		
		const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQuerryParams, QuerryParams);

		if (!bHit)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Exit: %s is NOT blocked."), *Exit->GetReadableName());
			OutExits.Add(Exit);
		}
		else
		{
			
			//UE_LOG(LogTemp, Warning, TEXT("Exit: %s is blocked by : %s"), *Exit->GetReadableName(), *HitResult.Actor->GetHumanReadableName());

		}		
	#if ENABLE_DRAW_DEBUG
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 55.0f, 0, 1);
	#endif
	}
}



