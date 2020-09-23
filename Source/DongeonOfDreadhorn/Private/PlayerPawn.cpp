// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VrOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VrOrigin"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	VrOrigin->SetupAttachment(RootComponent);
	Camera->SetupAttachment(VrOrigin);
	//Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPendingStep)
	{
		MakeStep();
	}

	if (bPendingTurn)
	{
		MakeTurn();
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("StepForward", IE_Pressed, this, &APlayerPawn::MoveForward);
	InputComponent->BindAction("StepRight", IE_Pressed, this, &APlayerPawn::MoveRight);
	InputComponent->BindAction("StepLeft", IE_Pressed, this, &APlayerPawn::MoveLeft);
	InputComponent->BindAction("TurnRight", IE_Pressed, this, &APlayerPawn::TurnRight);
	InputComponent->BindAction("TurnLeft", IE_Pressed, this, &APlayerPawn::TurnLeft);

}

void APlayerPawn::MoveForward()
{
	if (!bPendingStep && !bPendingTurn)
	{
		DesiredLocation = GetActorLocation() + (GetActorForwardVector() * StepDistance);
		if (CanMoveTo(DesiredLocation))
		{
			UE_LOG(LogTemp, Warning, TEXT("MoveForward"));
			bPendingStep = true;
		}
	}
}

void APlayerPawn::MoveRight()
{
	if (!bPendingStep && !bPendingTurn)
	{
		DesiredLocation = GetActorLocation() + (GetActorRightVector() * StepDistance);
		if (CanMoveTo(DesiredLocation))
		{
			UE_LOG(LogTemp, Warning, TEXT("MoveRight"));
			bPendingStep = true;
		}
	}
}

void APlayerPawn::MoveLeft()
{
	if (!bPendingStep && !bPendingTurn)
	{
		DesiredLocation = GetActorLocation() + ((GetActorRightVector() * -1) * StepDistance);
		if (CanMoveTo(DesiredLocation))
		{
			UE_LOG(LogTemp, Warning, TEXT("MoveLeft"));
			bPendingStep = true;
		}
	}
}

void APlayerPawn::TurnRight()
{
	if (!bPendingStep && !bPendingTurn)
	{
		UE_LOG(LogTemp, Warning, TEXT("TurnRight"));
		float NewYaw;
		if(GetActorRotation().Yaw + TurnAngle > 360)
		{
			NewYaw = 0.0f;
		}
		else
		{
			NewYaw = GetActorRotation().Yaw + TurnAngle;
		}
		DesiredRotation = FRotator(GetActorRotation().Pitch, NewYaw, GetActorRotation().Roll);
		bPendingTurn = true;
	}
}

void APlayerPawn::TurnLeft()
{
	if (!bPendingStep && !bPendingTurn)
	{
		UE_LOG(LogTemp, Warning, TEXT("TurnLeft"));
		float NewYaw;
		if (GetActorRotation().Yaw - TurnAngle < -360)
		{
			NewYaw = 0.0f;
		}
		else
		{
			NewYaw = GetActorRotation().Yaw - TurnAngle;
		}
		DesiredRotation = FRotator(GetActorRotation().Pitch, NewYaw, GetActorRotation().Roll);
		bPendingTurn = true;
	}
}

void APlayerPawn::MakeStep()
{
	FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), DesiredLocation, UGameplayStatics::GetWorldDeltaSeconds(this), StepInterpSpeed);

	if (SetActorLocation(NewLocation))
	{
		if ((DesiredLocation - GetActorLocation()).Size2D() <= StepReachDistance)
		{
			SetActorLocation(DesiredLocation);
			bPendingStep = false;
			UE_LOG(LogTemp, Warning, TEXT("Can Make Next Step: "));
		}
	}	
}

void APlayerPawn::MakeTurn()
{
	FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), DesiredRotation, UGameplayStatics::GetWorldDeltaSeconds(this), TurnInterpSpeed);

	if (SetActorRotation(NewRotation))
	{
		float DotProduct = FVector::DotProduct(GetActorRotation().Vector(), DesiredRotation.Vector());
		FVector CrossProduct = FVector::CrossProduct(GetActorRotation().Vector(), DesiredRotation.Vector());
		
		if (DotProduct >= TurnReachDistance)
		{
			SetActorRotation(DesiredRotation);
			bPendingTurn = false;
			UE_LOG(LogTemp, Warning, TEXT("Can Make Next Turn."));
		}
	}
}

bool APlayerPawn::CanMoveTo(FVector Destination)
{
	FVector TraceStart = GetActorLocation();	
	FVector TraceEnd = Destination;
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_Visibility;
	FCollisionQueryParams QuerryParams;
	QuerryParams.AddIgnoredActor(this);	
	return !GetWorld()->LineTraceTestByChannel(TraceStart, TraceEnd, TraceChannel, QuerryParams);
}


