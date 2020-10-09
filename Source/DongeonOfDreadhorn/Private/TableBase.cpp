// Fill out your copyright notice in the Description page of Project Settings.


#include "TableBase.h"
#include "Kismet/GamePlayStatics.h"
#include "PlayerPawn.h"

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
	PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));	
	GenerateCharactersPool();

	for(auto Character: Characters)
	{
		if(PlayerPawn)
		{
			int32 Index;
			PlayerPawn->AddCharacterToParty(Character, Index);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("@donkey@ Character: %s %s"), *Character.GeneralInfo.FirstName.ToString(), *Character.GeneralInfo.SecondName.ToString());
		}
	}
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

void ATableBase::StartEvent(FEventInfo EventInfo, const FOnEvenProcessed Callback)
{
	if (Dice)
	{
		//FOnValueDetermined::CreateUObject(this, &ATableBase::OnValueDetermined);
		Dice->Reset();
		Dice->OnValueDetermined.BindUObject(this, &ATableBase::OnValueDetermined, EventInfo, Callback);
		DODPlayerController->TravelToBoard();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dice is invalid"));
	}
}

void ATableBase::OnValueDetermined(int32 Value, FEventInfo EventInfo, const FOnEvenProcessed Callback)
{
	UE_LOG(LogTemp, Warning, TEXT("DeterminedValue: %d"), Value);
	bool Result = false;
	
	if(EventInfo.bAttributeBased)
	{
		int32 AttributeValue = 0;
		PlayerPawn->GetAttributeValue(EventInfo.Attribute, AttributeValue);
		AttributeValue = FMath::Clamp(AttributeValue, 1, 6);

		Result = EventInfo.bReversed ? Value >= 6 - AttributeValue : Value <= AttributeValue + 1;

		if (EventInfo.bReversed)
		{
			UE_LOG(LogTemp, Warning, TEXT("Determined value should be greater or equal than: %d"), 6 - AttributeValue);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Determined value should be less or equal than: %d"), AttributeValue + 1);
		}		
	}
	else
	{		
		Result = EventInfo.SuccessNumbers.Contains(Value);
		UE_LOG(LogTemp, Warning, TEXT("SuccessNumbers:"));
		for (auto Number : EventInfo.SuccessNumbers)
		{
			UE_LOG(LogTemp, Warning, TEXT("%d"), Number);
		}
	}
	
	//int32 SuccessThreshold = FMath::Clamp(AttributeValue, 1, 6)	;
	
	/*switch (EventInfo.Event)
	{
	case EEvent::Trap:
		break;
	case EEvent::Fight:
		Result = Value <= SuccessThreshold;
		break;
	case EEvent::Chest:
		break;
	default:
		break;
	}*/
	//Result = Value <= SuccessThreshold;
	Callback.ExecuteIfBound(Result);	
	DODPlayerController->TravelToDungeon();
}

void ATableBase::GenerateCharactersPool()
{
	Characters.Empty();
	if(CharactersDT)
	{
		static const FString ContextString(TEXT("GeneratingCharactersPool"));
		TArray<FCharacterInfo*> Rows;
		CharactersDT->GetAllRows<FCharacterInfo>(ContextString, Rows);
		if(Rows.Num() >= CharactersToGenerate)
		{
			for(int32 i = 0; i < CharactersToGenerate; i++)
			{
				int32 RandomIndex = FMath::RandRange(0, Rows.Num() - 1);
				FCharacterInfo Character = *Rows[RandomIndex];
				if(GetRandomName(Character.GeneralInfo.Race, Character.GeneralInfo.Gender, Character.GeneralInfo.FirstName, Character.GeneralInfo.SecondName))
				{
					Characters.Add(Character);
					Rows.RemoveAt(RandomIndex);					
				}				
			}
		}
	}
}

bool ATableBase::GetRandomName(ERace Race, EGender Gender, FText& OutFirstName, FText& OutSecondName)
{
	if(!CharactersNamesDT)
	{
		return false;
	}
	FName RowName;
	switch (Race)
	{
	case ERace::Human:
		RowName = HumanRowName;
		break;

	case ERace::Elf:
		RowName = ElvenRowName;
		break;

	case ERace::Dwarf:
		RowName = DwarvenRowName;
		break;		
	}
	if(RowName.IsNone())
	{
		return false;
	}
	static const FString ContextString(TEXT("GeneratingRandomNames"));
	FChacterNames* NamesRow = CharactersNamesDT->FindRow<FChacterNames>(RowName, ContextString);
	
	if (!NamesRow)
	{
		return false;
	}

	if(Gender == EGender::Male)
	{
		if (NamesRow->MaleFirstNames.Num() <= 0 || NamesRow->MaleSecondNames.Num() <= 0)
		{
			return false;
		}

		int32 RandomIndex = FMath::RandRange(0, NamesRow->MaleFirstNames.Num() - 1);
		OutFirstName = NamesRow->MaleFirstNames[RandomIndex];
		RandomIndex = FMath::RandRange(0, NamesRow->MaleSecondNames.Num() - 1);
		OutSecondName = NamesRow->MaleSecondNames[RandomIndex];
		return true;
	}

	if (NamesRow->FemaleFirstNames.Num() <= 0 || NamesRow->FemaleSecondNames.Num() <= 0)
	{
		return false;
	}

	int32 RandomIndex = FMath::RandRange(0, NamesRow->FemaleFirstNames.Num() - 1);
	OutFirstName = NamesRow->FemaleFirstNames[RandomIndex];
	RandomIndex = FMath::RandRange(0, NamesRow->FemaleSecondNames.Num() - 1);
	OutSecondName = NamesRow->FemaleSecondNames[RandomIndex];
	return true;	
}
