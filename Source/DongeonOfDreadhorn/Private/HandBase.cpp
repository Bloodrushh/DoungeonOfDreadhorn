// Fill out your copyright notice in the Description page of Project Settings.


#include "HandBase.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AHandBase::AHandBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionControllerComponent"));	
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MotionController->SetupAttachment(RootComponent);
	Mesh->SetupAttachment(MotionController);
}

// Called when the game starts or when spawned
void AHandBase::BeginPlay()
{
	Super::BeginPlay();

	if (Hand == EControllerHand::Left)
	{
		Mesh->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));
	}
	
}

// Called every frame
void AHandBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHandBase::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	MotionController->SetTrackingSource(Hand);
}

