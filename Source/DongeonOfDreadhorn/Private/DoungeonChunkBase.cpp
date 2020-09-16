// Fill out your copyright notice in the Description page of Project Settings.


#include "DoungeonChunkBase.h"

// Sets default values
ADoungeonChunkBase::ADoungeonChunkBase()
{

}

// Called when the game starts or when spawned
void ADoungeonChunkBase::BeginPlay()
{
	Super::BeginPlay();
	
	FindAndChachePossibleExits();
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
			UE_LOG(LogTemp, Warning, TEXT("Exit: %s is NOT blocked."), *Exit->GetReadableName());
			//FilteredExits.Add(Exit);
		}
		else
		{
			PossibleExits.Remove(Exit);
			UE_LOG(LogTemp, Warning, TEXT("Exit: %s is blocked"), *Exit->GetReadableName());
		}
	}

	if (PossibleExits.Num() > 0)
	{
		ValidExitIndex = FMath::RandRange(0, PossibleExits.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("ValidExitIndex: %d"), ValidExitIndex);
	}
}

bool ADoungeonChunkBase::TryGetValidExitSnapTransform(FTransform & OutTransform)
{
	ChooseValidExit();
	if (!PossibleExits.IsValidIndex(ValidExitIndex)) { return false; }
	OutTransform = PossibleExits[ValidExitIndex]->GetComponentTransform();
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

void ADoungeonChunkBase::GetPossibleExits(TArray<USceneComponent*>& OutPossibleExits)
{
	OutPossibleExits = PossibleExits;
}



