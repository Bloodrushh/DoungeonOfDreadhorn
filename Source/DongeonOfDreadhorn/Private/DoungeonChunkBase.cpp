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
	
	FindAndChachePossibleExits();
	UpdateValidExits();
	//UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	//GetComponents<UStaticMeshComponent>()
	FindAndCacheFloors();
	FindAndCacheChestSpawnPoints();
	FindAndCacheEnemySpawnPoints();	
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
		FTransform  SpawnTransform = Exit->GetComponentTransform();
		//UE_LOG(LogTemp, Warning, TEXT("Successfully found place for DeadEnd: x=%f, y=%f, z=%f"), SpawnTansform.GetLocation().X, SpawnTansform.GetLocation().Y, SpawnTansform.GetLocation().Z);
		ADoungeonChunkBase* DeadEnd = GetWorld()->SpawnActor<ADoungeonChunkBase>(DeadEndChunkClass, SpawnTransform, SpawnParams);
		if(DeadEnd)
		{
			PossibleExits.Remove(Exit);
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
		const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQueryParams, QueryParams);
		//const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, TraceStart, TraceEnd, ObjectQueryParams, QuerryParams);

		if (!bHit)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Exit: %s is NOT blocked."), *Exit->GetReadableName());
			OutExits.Add(Exit);
		}
		else
		{
			
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

void ADoungeonChunkBase::FindAndCacheEnemySpawnPoints()
{
	TArray<USceneComponent*> SceneComponents;
	GetComponents<USceneComponent>(SceneComponents, true);
	for (auto Component : SceneComponents)
	{
		if (Component->ComponentHasTag(EnemySpawnPointTag))
		{
			EnemySpawnPoints.Add(Component);			
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("EnemySpawnPoints.Num(): %d"), EnemySpawnPoints.Num());
}

void ADoungeonChunkBase::FindAndCacheChestSpawnPoints()
{
	TArray<USceneComponent*> SceneComponents;
	GetComponents<USceneComponent>(SceneComponents, true);
	for (auto Component : SceneComponents)
	{
		if (Component->ComponentHasTag(ChestSpawnPointTag))
		{
			ChestSpawnPoints.Add(Component);
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("EnemySpawnPoints.Num(): %d"), EnemySpawnPoints.Num())
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
	return CanSpawnEventTriggers && EventTriggers.Num() > 0;
}

bool ADoungeonChunkBase::SpawnEventTrigger()
{
	TSubclassOf<AEventTriggerBase> TriggerClass = EventTriggers[FMath::RandRange(0, EventTriggers.Num() - 1)];	
	if(TriggerClass)
	{
		UStaticMeshComponent* TriggerSpawnPoint = Floors[FMath::RandRange(0, Floors.Num() - 1)];
		if (TriggerSpawnPoint)
		{
			FTransform SpawnTransform = FTransform(FRotator(0.0f,0.0f,0.0f), TriggerSpawnPoint->GetComponentLocation());
			AEventTriggerBase* Trigger = GetWorld()->SpawnActorDeferred<AEventTriggerBase>(TriggerClass, SpawnTransform);
			if (Trigger)
			{
				Trigger->Chunk = this;
				Trigger->EnemySpawnPoints = EnemySpawnPoints;
				UGameplayStatics::FinishSpawningActor(Trigger, SpawnTransform);;
				return true;
			}
		}
	}
	return false;
}

void ADoungeonChunkBase::CloseExits()
{
	for(auto Exit: PossibleExits)
	{
		FVector Location = Exit->GetComponentLocation();
		FRotator Rotation = Exit->GetComponentRotation();
		ADoungeonChunkBase* Blocker = GetWorld()->SpawnActor<ADoungeonChunkBase>(ExitBlockerClass, Location, Rotation);
		if(Blocker)
		{
			ExitBlockers.Add(Blocker);
		}
	}
}

void ADoungeonChunkBase::OpenExits()
{
	for(auto Blocker : ExitBlockers)
	{
		Blocker->Destroy();
	}
	ExitBlockers.Empty();
}
