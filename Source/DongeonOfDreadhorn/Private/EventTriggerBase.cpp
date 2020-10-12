// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerBase.h"
#include "PlayerPawn.h"
#include "DrawDebugHelpers.h"
#include "DoungeonChunkBase.h"
#include "Kismet/GameplayStatics.h"
#include "TableBase.h"

// Sets default values
AEventTriggerBase::AEventTriggerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEventTriggerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEventTriggerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventTriggerBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	OnTriggered(OtherActor);
}

void AEventTriggerBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

bool AEventTriggerBase::TargetIsOnLineOfSight(AActor* InTarget)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(InTarget);
	if(PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("InTarget is a PlayerPawn"));
		FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 250.0f);
		FVector End = PlayerPawn->GetActorLocation();
		ECollisionChannel TraceChannel = ECollisionChannel::ECC_Visibility;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult HitResult;
		const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,TraceChannel);
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 55.0f, 0, 1);
		if(bHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Line Trace Succeed. Hit actor: %s"), *HitResult.Actor->GetHumanReadableName());
			return HitResult.Actor != nullptr ? HitResult.Actor == PlayerPawn : false;
		}		
	}
	return false;
}

void AEventTriggerBase::OnEventFinished()
{
	Chunk->OpenExits();
}

void
AEventTriggerBase::OnEventStarted()
{
	Chunk->CloseExits();
}

void AEventTriggerBase::SetIsTriggered(bool bTriggered)
{
	bIsTriggered = bTriggered;
}

void AEventTriggerBase::OnTriggered(AActor* TriggeredActor)
{
	UE_LOG(LogTemp, Warning, TEXT("OnTriggered called"));
	
	if(TargetIsOnLineOfSight(TriggeredActor) && !bIsTriggered)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can Trigger Event. Searching for Table"));
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(this, ATableBase::StaticClass(), FoundActors);
		if(FoundActors.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("FoundActors.Num() > 0"));
			ATableBase* Table = Cast<ATableBase>(FoundActors[0]);
			if(Table)
			{
				UE_LOG(LogTemp, Warning, TEXT("Table is valid. Binding functiong and starting event"));
				OnEventProcessedDelegate.BindDynamic(this, &AEventTriggerBase::OnEventProcessed);
				Table->StartEvent(EventInfo, OnEventProcessedDelegate);
				SetIsTriggered(true);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Table is not valid"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FoundActors.Num() <= 0"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannont trigger event. Target is out of line of sight or already triggered"));
	}
}


void AEventTriggerBase::OnEventProcessed(int32 DeterminedValue, APlayerPawn*& InPlayerPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEventProcceedTwo called"));
	UE_LOG(LogTemp, Warning, TEXT("DeterminedValue: %d called"), DeterminedValue);
	OnEventProcessedDelegate.Clear();
	OnEventProcessedBP();
	bool Result = false;

	if(!InPlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("InPlayerPawn is not valid"));
		FinishEvent(Result);
		return;		
	}
	
	if (EventInfo.SuccessNumbers.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EventInfo.SuccessNumbers.Num() <= 0"));
		FinishEvent(Result);
		return;
	}

	int32 AttributeValue = 0;
	InPlayerPawn->GetAttributeValue(EventInfo.Attribute, EventInfo.bGroup, AttributeValue);
	FCondition Condition = FCondition();
	int32 Difference = 0;
	
	UE_LOG(LogTemp, Warning, TEXT("EventInfo.SuccessNumbers.Num() > 0"));
	if (EventInfo.bAttributeBased)
	{
		UE_LOG(LogTemp, Warning, TEXT("EventInfo.bAttributeBased"));
		int32 BonusValue = 0;
		if (AttributeValue >= EventInfo.AttributeThreshold)
		{
			BonusValue = EventInfo.BonusValue;
			UE_LOG(LogTemp, Warning, TEXT("AttributeValue(%d) >= EventInfo.AttributeThreshold(%d). BonusValue: %d"), AttributeValue, EventInfo.AttributeThreshold, BonusValue);
		}

		int32 TotalValue = 0;
		Difference = FMath::Abs(EventInfo.SuccessNumbers[0] - TotalValue);
		UE_LOG(LogTemp, Warning, TEXT("Difference: %d"), Difference);
		if (EventInfo.bReversed)
		{
			UE_LOG(LogTemp, Warning, TEXT("EventInfo.bReversed"));
			TotalValue = FMath::Clamp(DeterminedValue - BonusValue, 1, 6);
			Result = TotalValue <= EventInfo.SuccessNumbers[0];
		}
		else
		{
			TotalValue = FMath::Clamp(DeterminedValue + BonusValue, 1, 6);
			Result = TotalValue >= EventInfo.SuccessNumbers[0];
		}
		Condition = Result ? EventInfo.SuccessConditions[FMath::Clamp(Difference, 0, 5)] : EventInfo.FailureConditions[FMath::Clamp(Difference - 1, 0, 5)];
	}
	else
	{
		if (EventInfo.SuccessNumbers.Num() == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("EventInfo.SuccessNumbers.Num() == 1. Value: %d"), EventInfo.SuccessNumbers[0]);
			Difference = FMath::Abs(EventInfo.SuccessNumbers[0] - DeterminedValue);
			UE_LOG(LogTemp, Warning, TEXT("Difference: %d"), Difference);
			if (EventInfo.bHardCondition)
			{
				UE_LOG(LogTemp, Warning, TEXT("EventInfo.bHardCondition"));
				Result = DeterminedValue == EventInfo.SuccessNumbers[0];
				Condition = Result ? EventInfo.SuccessConditions[0] : EventInfo.FailureConditions[FMath::Clamp(Difference - 1, 0, 5)];
			}
			else
			{
				if (EventInfo.bReversed)
				{
					Result = DeterminedValue <= EventInfo.SuccessNumbers[0];
				}
				else
				{
					Result = DeterminedValue >= EventInfo.SuccessNumbers[0];
				}
				Condition = Result ? EventInfo.SuccessConditions[FMath::Clamp(Difference, 0, 5)] : EventInfo.FailureConditions[FMath::Clamp(Difference - 1, 0, 5)];
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("EventInfo.SuccessNumbers:"));
			for(auto Number : EventInfo.SuccessNumbers)
			{
				UE_LOG(LogTemp, Warning, TEXT("%d"), Number);
			}
			Result = EventInfo.SuccessNumbers.Contains(DeterminedValue);
			Condition = Result ? EventInfo.SuccessConditions[0] : EventInfo.FailureConditions[0];
		}
	}
	switch (EventInfo.Event)
	{
	case EEvent::Fight:
		Chunk->CloseExits();
		// Do stuff like spawn enemies, comfigure their params, apply buffs/debuffs on player etc
		FinishEvent(Result);
		break;
	case EEvent::Trap:
		// Do stuff like dealing damage to player, apply buffs/debuffs on player etc		
		InPlayerPawn->ChangeAttributeValue(Condition.Effect, Condition.Attribute, Condition.Amount);
		FinishEvent(Result);
		break;;
	case EEvent::Chest:
		// Do stuff like giving reward to player, apply buffs/debuffs on player etc
		FinishEvent(Result);
		break;
	}	
}

void AEventTriggerBase::FinishEvent(bool bSucceed)
{
	switch (EventInfo.Event)
	{
	case EEvent::Fight:
		// Do stuff like open doors 
		Chunk->OpenExits();
		break;
	case EEvent::Trap:
		// Do stuff
		break;;
	case EEvent::Chest:
		// Do stuff
		break;
	}
	OnEventFinishedBP(bSucceed);
}

