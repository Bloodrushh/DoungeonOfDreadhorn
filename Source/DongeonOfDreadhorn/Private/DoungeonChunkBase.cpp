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
	UpdateValidExits();
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
}

void ADoungeonChunkBase::UpdateValidExits()
{
	ValidExits.Empty();

	for (auto Exit : PossibleExits)
	{
		FVector TraceStart = Exit->GetComponentTransform().GetLocation();
		float TraceLength = (GetRootComponent()->GetComponentTransform().GetLocation() - Exit->GetComponentTransform().GetLocation()).Size2D();
		FVector TraceEnd = TraceStart + (Exit->GetForwardVector() * TraceLength);
		ECollisionChannel TraceChannel = ECollisionChannel::ECC_Visibility;
		FCollisionQueryParams QuerryParams;
		QuerryParams.AddIgnoredActor(this);

		if (!GetWorld()->LineTraceTestByChannel(TraceStart, TraceEnd, TraceChannel, QuerryParams))
		{
			ValidExits.Add(Exit);
		}
	}
}

bool ADoungeonChunkBase::TryGetSpawnTransformForChunk(TSubclassOf<ADoungeonChunkBase> SpawningChunkClass, FTransform & OutTransform)
{
	UpdateValidExits();
	//DEBUG
	/*for (auto Exit : ValidExits)
	{
		UE_LOG(LogTemp, Warning, TEXT("ValidExit: %s"), *Exit->GetFullName());
	}*/
	

	ADoungeonChunkBase* ChunkCDO = SpawningChunkClass.GetDefaultObject();
	TArray<USceneComponent*> Floors;
	TArray<USceneComponent*> AvailableExits;

	UWorld* World = GetWorld();

	for (auto ChildComponent : GetComponentsByTag(USceneComponent::StaticClass(), FloorTag))
	{
		//ChildComponent->IsA<USceneComponent>();
		USceneComponent* SceneComponent = Cast<USceneComponent>(ChildComponent);
		if (SceneComponent)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Added Floor: %s"), *SceneComponent->GetFullName());
			Floors.Add(SceneComponent);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Component is not a USceneComponent: %s"), *SceneComponent->GetClass()->GetFullName());
		}
	}

	//DEBUG
	/*for (auto Floor : Floors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Floor: %s"), *Floor->GetFullName());
	}*/

	for (auto Exit : ValidExits)
	{
		bool bFoundAvailabeExit = false;
		FTransform ExitTransform = Exit->GetComponentTransform();
		
		for (auto Floor : Floors)
		{
			FVector End = ExitTransform.TransformPosition(Floor->GetComponentLocation());
			FVector Start = End + FVector(0.0f, 0.0f, 250);
			FHitResult HitResult;

#if ENABLE_DRAW_DEBUG
			DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 99.0f, 0, 1);
#endif
			
			if (World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
			{
				//UE_LOG(LogTemp, Warning, TEXT("HitSomething at the location: X=%f. Y=%f, Z=%f"), End.X, End.Y, End.Z);
				bFoundAvailabeExit = false;
				break;
			}
			bFoundAvailabeExit = true;
		}

		if (bFoundAvailabeExit)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Added exit to AvailableExits: %s"), *Exit->GetFullName());
			AvailableExits.Add(Exit);
		}
	}

	if (AvailableExits.Num() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("AvailableExits.Num() > 0"));
		if (AvailableExits.Num() > 1)
		{
			//UE_LOG(LogTemp, Warning, TEXT("AvailableExits.Num() > 1"));
			int32 ExitIndex = FMath::RandRange(0, AvailableExits.Num() - 1);
			OutTransform = AvailableExits[ExitIndex]->GetComponentTransform();
		}

		OutTransform = AvailableExits[0]->GetComponentTransform();
		return true;
	}
	
	/*if (!ValidExits.IsValidIndex(ValidExitIndex)) { return false; }
	OutTransform = ValidExits[ValidExitIndex]->GetComponentTransform();
	return true;*/
	//UE_LOG(LogTemp, Warning, TEXT("AvailableExits.Num() <= 0"));
	return false;
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
		/*float TraceLength = ((GetRootComponent()->GetComponentTransform().GetLocation() - Exit->GetComponentTransform().GetLocation()).Size2D() / 2) - 50;
		FVector TraceStart = Exit->GetComponentTransform().GetLocation();		
		FVector TraceEnd = Exit->GetComponentTransform().GetLocation() + ((Exit->GetForwardVector()* -1) * TraceLength);*/

		
		FVector TraceEnd = Exit->GetComponentTransform().GetLocation();
		FVector TraceStart = TraceEnd + FVector(0.0f, 0.0f, 250.f);

		ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldStatic;
		FCollisionQueryParams QuerryParams;
		FCollisionObjectQueryParams ObjectQuerryParams;
		FHitResult HitResult;		
		const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQuerryParams, QuerryParams);
		//const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQuerryParams, QuerryParams);

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



