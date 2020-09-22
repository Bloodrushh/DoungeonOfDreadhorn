// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VrOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VrOrigin"));
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	VrOrigin->SetupAttachment(RootComponent);
	Camera->SetupAttachment(VrOrigin);
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
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


	//UE_LOG(LogTemp, Warning, TEXT("OurLocation: x=%f, y=%f, z=%f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z);
	//UE_LOG(LogTemp, Warning, TEXT("OurRotation: Pitch=%f, Yaw=%f, Roll=%f"), GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll);
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
		//UE_LOG(LogTemp, Warning, TEXT("Current Yaw: %f, New Yaw: %f"), GetActorRotation().Yaw, NewYaw);
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
		//UE_LOG(LogTemp, Warning, TEXT("Current Yaw: %f, New Yaw: %f"), GetActorRotation().Yaw, NewYaw);
		DesiredRotation = FRotator(GetActorRotation().Pitch, NewYaw, GetActorRotation().Roll);
		bPendingTurn = true;
	}
}

void APlayerPawn::MakeStep()
{
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), DesiredLocation, UGameplayStatics::GetWorldDeltaSeconds(this), StepInterpSpeed);
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
	//UE_LOG(LogTemp, Warning, TEXT("OurRotation: Pitch=%f, Yaw=%f, Roll=%f"), GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll);
	//UE_LOG(LogTemp, Warning, TEXT("DesiredRotation: Pitch=%f, Yaw=%f, Roll=%f"), DesiredRotation.Pitch, DesiredRotation.Yaw, DesiredRotation.Roll);
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), DesiredRotation, UGameplayStatics::GetWorldDeltaSeconds(this), TurnInterpSpeed);

	//UE_LOG(LogTemp, Warning, TEXT("NewRotation: Pitch=%f, Yaw=%f, Roll=%f"), NewRotation.Pitch, NewRotation.Yaw, NewRotation.Roll);
	/*FRotator DeltaRotator = FRotator(0.0f, 90 * UGameplayStatics::GetWorldDeltaSeconds(this), 0.0f);
	AddActorWorldRotation(DeltaRotator);*/
	if (SetActorRotation(NewRotation))//SetActorRotation(NewRotation)
	{
		//AddActorWorldRotation(DeltaRotator);
		//UE_LOG(LogTemp, Warning, TEXT("Turn Distance: %f"), FMath::Abs((GetActorRotation().Yaw - DesiredRotation.Yaw)));
		float DotProduct = FVector::DotProduct(GetActorRotation().Vector(), DesiredRotation.Vector());
		if (DotProduct >= TurnReachDistance)//FMath::Abs((DesiredRotation.Yaw - GetActorRotation().Yaw)) <= TurnReachDistance		DesiredRotation.Yaw - GetActorRotation().Yaw <= TurnReachDistance
		{
			SetActorRotation(DesiredRotation);
			bPendingTurn = false;
			UE_LOG(LogTemp, Warning, TEXT("Can Make Next Turn."));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DotProduct: %f"), DotProduct);
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


