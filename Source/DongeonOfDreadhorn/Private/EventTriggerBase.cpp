// Fill out your copyright notice in the Description page of Project Settings.


#include "EventTriggerBase.h"
#include "PlayerPawn.h"
#include "DrawDebugHelpers.h"
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

