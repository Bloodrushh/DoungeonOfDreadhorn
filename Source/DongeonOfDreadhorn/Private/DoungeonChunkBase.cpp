// Fill out your copyright notice in the Description page of Project Settings.


#include "DoungeonChunkBase.h"
#include "DrawDebugHelpers.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/SCS_Node.h"
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
	TArray<USceneComponent*> Floors;
	UBlueprintGeneratedClass* ParentBluepinrtClass = Cast<UBlueprintGeneratedClass>(SpawningChunkClass->GetSuperClass());
	const TArray<USCS_Node*>& ParentActorBlueprintNodes = ParentBluepinrtClass->SimpleConstructionScript->GetAllNodes();
	UE_LOG(LogTemp, Warning, TEXT("Trying to get floors from parent class: %s"), *SpawningChunkClass->GetFullName());

	for (USCS_Node* Node : ParentActorBlueprintNodes)
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
					Floors.Add(SceneComponent);
				}
			}
		}
	}

	UBlueprintGeneratedClass* ChildBluepinrtClass = Cast<UBlueprintGeneratedClass>(SpawningChunkClass);
	const TArray<USCS_Node*>& ChildActorBlueprintNodes = ChildBluepinrtClass->SimpleConstructionScript->GetAllNodes();
	UE_LOG(LogTemp, Warning, TEXT("Trying to get floors from child class: %s"), *SpawningChunkClass->GetFullName());
	
	for (USCS_Node* Node : ChildActorBlueprintNodes)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checnkin Node class: %s"), *Node->ComponentClass->GetFullName());
		if (Node->ComponentClass == UStaticMeshComponent::StaticClass())
		{
			//Node->GetActualComponentTemplate()
			//Node ->get
			UE_LOG(LogTemp, Warning, TEXT("FoundStaticMeshComponent"));
			USceneComponent* SceneComponent = Cast<USceneComponent>(Node->ComponentTemplate);
			if (SceneComponent)
			{
				if (SceneComponent->ComponentHasTag(FloorTag))
				{
					UE_LOG(LogTemp, Warning, TEXT("FoundFloor"));
					Floors.Add(SceneComponent);
				}				
			}
		}
	}

	//Uncommnent
	TArray<USceneComponent*> AvailableExits;
	UWorld* World = GetWorld();	
	UE_LOG(LogTemp, Warning, TEXT("Floors amount: %d"), Floors.Num());

	for (auto Exit : ValidExits)
	{
		bool bFoundAvailabeExit = false;
		FTransform ExitTransform = Exit->GetComponentTransform();
		
		for (auto Floor : Floors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Class: %s, Checking Floor: %s"), *SpawningChunkClass->GetFullName(), *Floor->GetFullName());
			FVector End = ExitTransform.TransformPosition(Floor->GetRelativeLocation());
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
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 55.0f, 0, 1);
	}
}



