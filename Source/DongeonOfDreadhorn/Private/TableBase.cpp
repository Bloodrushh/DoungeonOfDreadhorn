// Fill out your copyright notice in the Description page of Project Settings.


#include "TableBase.h"
#include "Kismet/GamePlayStatics.h"

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
	
	DODPlayerController = Cast<ADODPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
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

void ATableBase::StartEvent(EEventType Event, EAttributeType Attribute, const FOnEvenProcessed Callback)
{
	if (Dice)
	{
		//FOnValueDetermined::CreateUObject(this, &ATableBase::OnValueDetermined);
		Dice->Reset();
		Dice->OnValueDetermined.BindUObject(this, &ATableBase::OnValueDetermined, Event, Attribute, Callback);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dice is invalid"));
	}
	/*bool Result = false;
	switch (Event)
	{
	case EEventType::Trap:
		break;
	case EEventType::Fight:
		Result = true;
		break;
	case EEventType::Chest:
		break;
	default:
		break;
	}

	
	Callback.ExecuteIfBound(Result);*/	
}

void ATableBase::OnValueDetermined(int32 Value, EEventType Event, EAttributeType Attribute, const FOnEvenProcessed Callback)
{
	UE_LOG(LogTemp, Warning, TEXT("DeterminedValue: %d"), Value);

	bool Result = false;
	switch (Event)
	{
	case EEventType::Trap:
		break;
	case EEventType::Fight:
		Result = Attribute == EAttributeType::Strength && Value >= 3;
		break;
	case EEventType::Chest:
		break;
	default:
		break;
	}
	Callback.ExecuteIfBound(Result);
	// Вот сюда юы мне ссылка на callback
}

