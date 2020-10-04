// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPawnHand.h"
#include "PickupableInterface.h"

ABoardPawnHand::ABoardPawnHand()
{
	UE_LOG(LogTemp, Warning, TEXT("ABoardPawnHand constructor"));
	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
	GrabSphere->SetupAttachment(Mesh);
}

void ABoardPawnHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Grab();
}

void ABoardPawnHand::Grab()
{
	AActor* OutActor = nullptr;
	//OutActor->GetClass()->ImplementsInterface(UPickupableInterface::StaticClass());

	if (GetNearestActor(OutActor))
	{
		if (OutActor->Implements<UPickupableInterface>())
		{			
			IPickupableInterface* PickupableActor = Cast<IPickupableInterface>(OutActor);
			if (PickupableActor)
			{
				PickupableActor->OnPickup(Mesh);
				AttachedActor = OutActor;
				UE_LOG(LogTemp, Warning, TEXT("I am: %s | Trying to pickup: %s"), *GetHumanReadableName(), *OutActor->GetHumanReadableName());
			}			
		}		
	}
}

bool ABoardPawnHand::GetNearestActor(AActor*& OutActor)
{
	TArray<AActor*> OverlappingActors;
	GrabSphere->GetOverlappingActors(OverlappingActors);
	AActor* NearsActor = nullptr;
	for (auto Actor : OverlappingActors)
	{		
		if (!NearsActor)
		{
			NearsActor = Actor;
		}
		else if((Actor->GetActorLocation() - GetActorLocation()).Size() < (NearsActor->GetActorLocation() - GetActorLocation()).Size())
		{
			NearsActor = Actor;
		}
	}
	OutActor = NearsActor;
	return NearsActor != nullptr;
}

void ABoardPawnHand::Drop()
{
	if (AttachedActor)
	{
		IPickupableInterface* PickupableActor = Cast<IPickupableInterface>(AttachedActor);
		if (PickupableActor)
		{
			PickupableActor->OnDrop();
			UE_LOG(LogTemp, Warning, TEXT("I am: %s | Trying to Drop: %s"), *GetHumanReadableName(), *AttachedActor->GetHumanReadableName());
		}
	}
}
	
