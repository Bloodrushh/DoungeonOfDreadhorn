// Fill out your copyright notice in the Description page of Project Settings.


#include "FootPrintDecal.h"
#include "Components/DecalComponent.h"

// Sets default values
AFootPrintDecal::AFootPrintDecal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	DecalComponent->SetDecalMaterial(DecalMaterial);
	DecalComponent->SetRelativeRotation(FRotator(-90, 0, 0));
}

// Called when the game starts or when spawned
void AFootPrintDecal::BeginPlay()
{
	Super::BeginPlay();
	
}


