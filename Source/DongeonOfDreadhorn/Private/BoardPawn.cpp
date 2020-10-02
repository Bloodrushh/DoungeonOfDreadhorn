// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPawn.h"
#include "Camera/CameraComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABoardPawn::ABoardPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	VrOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VrOrigin"));
	VrOrigin->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(VrOrigin);
}

// Called when the game starts or when spawned
void ABoardPawn::BeginPlay()
{
	Super::BeginPlay();
	
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	if (HandClass)
	{
		FAttachmentTransformRules AttachmentRuesl = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
		FTransform SpawnTransform = GetActorTransform();

		RightHand = GetWorld()->SpawnActorDeferred<AHandBase>(HandClass, SpawnTransform, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		RightHand->Hand = EControllerHand::Right;		
		UGameplayStatics::FinishSpawningActor(RightHand, SpawnTransform);
		RightHand->AttachToComponent(VrOrigin, AttachmentRuesl);

		LeftHand = GetWorld()->SpawnActorDeferred<AHandBase>(HandClass, SpawnTransform, this, this, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		LeftHand->Hand = EControllerHand::Left;		
		UGameplayStatics::FinishSpawningActor(LeftHand, SpawnTransform);
		LeftHand->AttachToComponent(VrOrigin, AttachmentRuesl);
	}
}

// Called every frame
void ABoardPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoardPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

