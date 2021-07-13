// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "HandBase.h"
#include "FootPrintDecal.h"
#include "DODPlayerController.h"

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

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
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

void APlayerPawn::Die_Implementation()
{
	
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
	if (!bPendingStep && !bPendingTurn && bCanMove)
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
	if (!bPendingStep && !bPendingTurn && bCanMove)
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
	if (!bPendingStep && !bPendingTurn && bCanMove)
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
			SpawnFootPrint();
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

void APlayerPawn::SpawnFootPrint()
{
	FHitResult HitResult;
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_Visibility;
	FCollisionQueryParams QuerryParams;	
	QuerryParams.AddIgnoredActor(this);
	QuerryParams.AddIgnoredActor(LeftHand);
	QuerryParams.AddIgnoredActor(RightHand);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() - FVector(0.0f, 0.0f, 250.f), TraceChannel, QuerryParams))
	{
		FVector Location = HitResult.ImpactPoint;
		FRotator Rotation = FRotator();
		FActorSpawnParameters SpawnParammeters;
		AFootPrintDecal* SpawnedFoorPrint = GetWorld()->SpawnActor<AFootPrintDecal>(FootPrintDecalClass, Location, Rotation, SpawnParammeters);
		if (SpawnedFoorPrint)
		{
			if (FootPrints.Num() >= MaxFootPrints)
			{
				AFootPrintDecal* FoorPrintToRemove = FootPrints[0];
				FootPrints.Remove(FoorPrintToRemove);
				FoorPrintToRemove->Destroy();
			}
			FootPrints.Add(SpawnedFoorPrint);
		}
	}	
}

void APlayerPawn::DisableControllers()
{
	if (!RightHand || !LeftHand)
	{
		return;
	}
	
	RightHand->MotionController->Deactivate();
	LeftHand->MotionController->Deactivate();
}

void APlayerPawn::EnableControllers()
{
	if (!RightHand || !LeftHand)
	{
		return;
	}
	RightHand->MotionController->Activate();
	LeftHand->MotionController->Activate();
}

void APlayerPawn::AddCharacterToParty(FCharacterInfo InCharacter, int32& OutIndex)
{
	OutIndex = Characters.Add(InCharacter);
	UE_LOG(LogTemp, Warning, TEXT("@DONKEY@ Successfully added character: FISRTNAME=%s , SECONDNAME=%s to party. Index: %d"), *InCharacter.GeneralInfo.FirstName.ToString(), *InCharacter.GeneralInfo.SecondName.ToString(),  OutIndex);
}

bool APlayerPawn::RemoveCharacterFromParty(int32 InIndex)
{
	if(Characters.IsValidIndex(InIndex))
	{
		Characters.RemoveAt(InIndex);
		return true;
	}	
	return false;
}

void APlayerPawn::GetAttributeValue(EAttribute InAttribute, bool bGroup, int32& OutValue)
{
	if(bGroup)
	{
		int32 LocalValue = 0;
		for (auto Character : Characters)
		{
			OutValue += *Character.AttributesInfo.Attributes.Find(InAttribute);
		}
	}
	else
	{
		OutValue = *Characters[ActiveCharacterIndex].AttributesInfo.Attributes.Find(InAttribute);
	}	
}

void APlayerPawn::ChangeAttributeValue(EEffect Effect, EAttribute Attribute, int32 inValue)
{
	int32* AttributeValue = Characters[ActiveCharacterIndex].AttributesInfo.Attributes.Find(Attribute);
	int32 NewValue = *AttributeValue;

	// for debug
	FString StringAttribute;
	FString StringEffect;
	
	switch (Effect)
	{
	case EEffect::Add:
		StringEffect = TEXT("Added");
		NewValue += inValue;
		break;
	case EEffect::Subtract:
		StringEffect = TEXT("Subtracted");
		NewValue -= inValue;
		break;
	case EEffect::Multiply:
		StringEffect = TEXT("Multiplied");
		NewValue *= inValue;
		break;
	case EEffect::Divide:
		StringEffect = TEXT("Divided");
		NewValue /= inValue;
		break;
	case EEffect::None:
		break;
	}
	// for debug
	switch (Attribute)
	{
	case EAttribute::Health:
		StringAttribute = TEXT("Health");
		break;
	case EAttribute::Damage:
		StringAttribute = TEXT("Damage");
		break;
	case EAttribute::ActionPoints:
		StringAttribute = TEXT("ActionPoints");
		break;
	case EAttribute::Initiative:
		StringAttribute = TEXT("Initiative");
		break;
	case EAttribute::Strength:
		StringAttribute = TEXT("Strength");
		break;
	case EAttribute::Agility:
		StringAttribute = TEXT("Agility");
		break;
	case EAttribute::Intelligence:
		StringAttribute = TEXT("Intelligence");
		break;
	}

	UE_LOG(LogTemp, Warning, TEXT("Attribute: %s was changed for Character: \"%s - %s\" by Effect: %s by value: %d. Old value: %d, New value: %d"),
	       *StringAttribute, *Characters[ActiveCharacterIndex].GeneralInfo.FirstName.ToString(), *Characters[ActiveCharacterIndex].GeneralInfo.SecondName.ToString(), *StringEffect, inValue, *AttributeValue, NewValue);
	
	*AttributeValue = NewValue;
}

void APlayerPawn::TakeDamage(int32 Amount, EAttack Attack)
{
	OnDamageTakenBP(Amount, Attack);
	
	// decrement current character health	
	auto HealthAttribute = *Characters[ActiveCharacterIndex].AttributesInfo.Attributes.Find(EAttribute::Health);
	HealthAttribute -= Amount;

	if (HealthAttribute <= 0)
	{
		Die();
	}	
}

bool APlayerPawn::CanTakeDamage()
{
	return true;
}

int32 APlayerPawn::GetInitiative()
{	
	int32 OutInitiative = 0;
	for (auto Character : Characters)
	{
		OutInitiative += *Character.AttributesInfo.Attributes.Find(EAttribute::Initiative);
		UE_LOG(LogTemp, Warning, TEXT("Initiative: %d"), OutInitiative);
	}

	UE_LOG(LogTemp, Warning, TEXT("APlayerPawn::GetInitiative: %d"), OutInitiative);
	return OutInitiative;
}

void APlayerPawn::PerformMove()
{
	UE_LOG(LogTemp, Warning, TEXT("APlayerPawn::PerformMove called"));
}

void APlayerPawn::OnMovePerformed()
{
	
}

void APlayerPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	EnableControllers();
}

void APlayerPawn::UnPossessed()
{
	Super::UnPossessed();
	DisableControllers();
}

void APlayerPawn::TeleportIntoFight(FVector Location)
{
	CachedDungeonTransform = GetActorTransform();
	TeleportTo(Location, FRotator(0,0,0));
}

void APlayerPawn::TeleportIntoDungeon()
{
	TeleportTo(CachedDungeonTransform.GetLocation(), CachedDungeonTransform.GetRotation().Rotator());
	ToggleCanMove(true);
}

void APlayerPawn::ToggleCanMove(bool bCan)
{
	bCanMove = bCan;
}
