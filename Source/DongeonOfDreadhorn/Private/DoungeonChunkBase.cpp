// Fill out your copyright notice in the Description page of Project Settings.


#include "DoungeonChunkBase.h"
#include "DrawDebugHelpers.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"
#include "Kismet/GameplayStatics.h"
#include "EventTriggerBase.h"
// Sets default values
ADoungeonChunkBase::ADoungeonChunkBase()
{

}

// Called when the game starts or when spawned
void ADoungeonChunkBase::BeginPlay()
{
	Super::BeginPlay();
	
	FindAndCachePossibleExits();
	UpdateValidExits();
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	//GetComponents<UStaticMeshComponent>()
	FindAndCacheFloors();
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
	TArray<USceneComponent*> FloorsComponents;
	GetFloorsFromChunkClass(SpawningChunkClass, FloorsComponents);
	TArray<USceneComponent*> AvailableExits;
	UWorld* World = GetWorld();	
	UE_LOG(LogTemp, Warning, TEXT("Floors amount: %d"), Floors.Num());

	for (auto Exit : ValidExits)
	{
		bool bFoundAvailabeExit = false;
		FTransform ExitTransform = Exit->GetComponentTransform();
		
		for (auto FloorComponent : FloorsComponents)
		{
			UE_LOG(LogTemp, Warning, TEXT("Class: %s, Checking Floor: %s"), *SpawningChunkClass->GetFullName(), *FloorComponent->GetFullName());
			FVector End = ExitTransform.TransformPosition(FloorComponent->GetRelativeLocation());
			FVector Start = End + FVector(0.0f, 0.0f, 250);
			FHitResult HitResult;

			//DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 99.0f, 0, 5);

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
	return false;
}

void ADoungeonChunkBase::FindAndCachePossibleExits()
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
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (auto Exit : DeadExits)
	{	
		FTransform  SpawnTransform = Exit->GetComponentTransform();
		//UE_LOG(LogTemp, Warning, TEXT("Successfully found place for DeadEnd: x=%f, y=%f, z=%f"), SpawnTansform.GetLocation().X, SpawnTansform.GetLocation().Y, SpawnTansform.GetLocation().Z);
		ADoungeonChunkBase* DeadEnd = GetWorld()->SpawnActor<ADoungeonChunkBase>(DeadEndChunkClass, SpawnTransform, SpawnParams);
		if(DeadEnd)
		{
			PossibleExits.Remove(Exit);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("@DONKEY@ Could not spawn Deadend"));
		}
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
		FCollisionQueryParams QueryParams;
		FCollisionObjectQueryParams ObjectQueryParams;
		FHitResult HitResult;		
		bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQueryParams, QueryParams);
		//const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQueryParams, QuerryParams);

		if (!bHit)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Exit: %s is NOT blocked."), *Exit->GetReadableName());
			OutExits.Add(Exit);
		}
		else
		{
			// this fixes problem when dead ends could not spawn back to back with other wall
			TraceEnd = TraceStart + (-Exit->GetComponentRotation().Vector() * 250.0f);
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 55.0f, 0, 1);
			bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQueryParams, QueryParams);
			if(bHit)
			{
				OutExits.Add(Exit);
				//UE_LOG(LogTemp, Warning, TEXT("@DONKEY@ Exit: %s"), *Exit->GetReadableName());
				//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 99.0f, 0, 1);
			}
			//UE_LOG(LogTemp, Warning, TEXT("Exit: %s is blocked by : %s"), *Exit->GetReadableName(), *HitResult.Actor->GetHumanReadableName());

		}			
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 55.0f, 0, 1);
	}
}

void ADoungeonChunkBase::GetFloorsFromChunkClass(TSubclassOf<ADoungeonChunkBase> ChunkClass, TArray<USceneComponent*>& OutFloors)
{
	UBlueprintGeneratedClass* BlueprintClass = Cast<UBlueprintGeneratedClass>(ChunkClass);
	do
	{
		if (!BlueprintClass)
		{
			return;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BluepinrtClass is valid: %s"), *BlueprintClass->GetFullName());
		}

		const TArray<USCS_Node*>& ActorBlueprintNodes = BlueprintClass->SimpleConstructionScript->GetAllNodes();
		UE_LOG(LogTemp, Warning, TEXT("Trying to get floors from class: %s"), *BlueprintClass->GetFullName());

		for (USCS_Node* Node : ActorBlueprintNodes)
		{
			UE_LOG(LogTemp, Warning, TEXT("Checnkin Node class: %s"), *Node->ComponentClass->GetFullName());
			if (Node->ComponentClass == UStaticMeshComponent::StaticClass())
			{
				UE_LOG(LogTemp, Warning, TEXT("FoundStaticMeshComponent"));
				USceneComponent* SceneComponent = Cast<USceneComponent>(Node->ComponentTemplate);
				if (SceneComponent)
				{
					if (SceneComponent->ComponentHasTag(FloorTag))
					{
						UE_LOG(LogTemp, Warning, TEXT("FoundFloor"));
						OutFloors.Add(SceneComponent);
					}
				}
			}
		}

		BlueprintClass = Cast<UBlueprintGeneratedClass>(BlueprintClass->GetSuperClass());
		UE_LOG(LogTemp, Warning, TEXT("New BluepinrtClass: %s"), *BlueprintClass->GetFullName());
		
	} while (BlueprintClass != AActor::StaticClass() && BlueprintClass);	
}

void ADoungeonChunkBase::OnVisited()
{
	bVisited = true;
	for (auto Chunk : BoardChunks)
	{
		Chunk->OnVisited();
	}
}

void ADoungeonChunkBase::AddBoardChunk(ADungeonChunkBoardBase * NewBoardChunk)
{
	BoardChunks.Add(NewBoardChunk);
}

void ADoungeonChunkBase::FindAndCacheFloors()
{
	TArray<UStaticMeshComponent*> StaticMeshes;
	GetComponents<UStaticMeshComponent>(StaticMeshes, true);
	for (auto Mesh : StaticMeshes)
	{
		if (Mesh->ComponentHasTag(FloorTag))
		{
			if (Mesh->IsCollisionEnabled())
			{
				Floors.Add(Mesh);
			}
		}
	}
}

bool ADoungeonChunkBase::GetCanSpawnEventTriggers()
{
	return CanSpawnEventTriggers && EventTriggerClasses.Num() > 0;
}

bool ADoungeonChunkBase::SpawnEventTrigger()
{
	if (EventTriggerClasses.Num() <= 0)
	{
		return false;
	}
	
	TSubclassOf<AEventTriggerBase> TriggerClass = EventTriggerClasses[FMath::RandRange(0, EventTriggerClasses.Num() - 1)].LoadSynchronous();	
	if(TriggerClass)
	{
		UStaticMeshComponent* TriggerSpawnPoint = Floors[FMath::RandRange(0, Floors.Num() - 1)];
		if (TriggerSpawnPoint)
		{
			FTransform SpawnTransform = FTransform(FRotator(0.0f,0.0f,0.0f), TriggerSpawnPoint->GetComponentLocation());
			AEventTriggerBase* Trigger = GetWorld()->SpawnActorDeferred<AEventTriggerBase>(TriggerClass, SpawnTransform);
			if (Trigger)
			{
				EventTriggers.Add(Trigger);
				Trigger->Chunk = this;
				Trigger->PlayerPawn = PlayerPawn;
				Trigger->DungeonManager = DungeonManager;
				UGameplayStatics::FinishSpawningActor(Trigger, SpawnTransform);;
				return true;
			}
		}
	}
	return false;
}

void ADoungeonChunkBase::Disappear_Implementation()
{
	for (auto Trigger : EventTriggers)
	{
		Trigger->Disappear();
	}
	Destroy();
}

void ADoungeonChunkBase::SetPlayerPawn(APlayerPawn* InPlayerPawn)
{
	PlayerPawn = InPlayerPawn;
}

void ADoungeonChunkBase::SetDungeonManager(ADungeonManager* InDungeonManager)
{
	DungeonManager = InDungeonManager;
}
