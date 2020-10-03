// Fill out your copyright notice in the Description page of Project Settings.


#include "DiceBase.h"
#include "Engine/CollisionProfile.h"
// Sets default values
ADiceBase::ADiceBase() //(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)	
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));	
	Mesh->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	Mesh->SetSimulatePhysics(true);
	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ADiceBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<UActorComponent*> ActorComponents;
	GetComponents(ActorComponents);
	for (auto Component : ActorComponents)
	{
		if (Component->ComponentHasTag(TEXT("DiceFace")))
		{
			UDiceFaceComponent* DiceFaceComponent = Cast<UDiceFaceComponent>(Component);
			if (DiceFaceComponent)
			{
				DiceFaces.Add(DiceFaceComponent);
				UE_LOG(LogTemp, Warning, TEXT("Found dice face: %d"), DiceFaceComponent->FaceValue);
			}
		}
	}
	
}

// Called every frame
void ADiceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bThrown && !bValueDetermined)
	{
		if (GetVelocity().Size() <= 0)
		{
			UDiceFaceComponent* Best;
			DetermineValue(Best);
			bValueDetermined = true;
			UE_LOG(LogTemp, Warning, TEXT("DeterminedValue: %d"), Best->FaceValue);
			
		}
	}

}

void ADiceBase::OnPickup(USceneComponent * AttachTo)
{
	Mesh->SetSimulatePhysics(false);
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	AttachToComponent(AttachTo, AttachmentRules);
}

void ADiceBase::OnDrop()
{	
	Mesh->SetSimulatePhysics(true);
	FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, false);
	DetachFromActor(DetachmentRules);
	bThrown = true;
	bValueDetermined = false;
}

void ADiceBase::DetermineValue(UDiceFaceComponent*& OutDiceFace)
{
	UDiceFaceComponent* Best = nullptr;
	for (auto Face : DiceFaces)
	{
		if (!Best)
		{
			Best = Face;
		}
		else
		{
			float BestDot = FVector::DotProduct(FVector::UpVector, Best->GetForwardVector());
			float FaceDot = FVector::DotProduct(FVector::UpVector, Face->GetForwardVector());
			if (FaceDot > BestDot)
			{
				Best = Face;
			}			
		}
	}
	OutDiceFace = Best;
}

