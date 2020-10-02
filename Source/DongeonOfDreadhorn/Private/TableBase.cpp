// Fill out your copyright notice in the Description page of Project Settings.


#include "TableBase.h"

// Sets default values
ATableBase::ATableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATableBase::GetBounce(float & OutMaxX, float & OutMinX, float & OutMaxY, float & OutMinY)
{
	FVector Location = GetActorLocation();
	OutMaxX = Location.X + Height;
	OutMinX = Location.X - Height;
	OutMaxY = Location.Y + Width;
	OutMinY = Location.Y - Width;
}

