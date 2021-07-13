

#include "TableBase.h"
#include "Kismet/GamePlayStatics.h"
#include "PlayerPawn.h"
#include "EventTriggerBase.h"
#include "Components/WidgetComponent.h"
#include "EventWidget.h"
// Sets default values
ATableBase::ATableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.	
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	TestEventWindow = CreateDefaultSubobject<UWidgetComponent>(TEXT("TestEventInfo"));
	TestEventWindow->SetupAttachment(RootComponent);
	TestEventWindow->SetWidgetClass(UEventWidget::StaticClass());
}

// Called when the game starts or when spawned
void ATableBase::BeginPlay()
{
	Super::BeginPlay();
}


void ATableBase::GetBounce(float & OutMaxX, float & OutMinX, float & OutMaxY, float & OutMinY)
{
	FVector Location = GetActorLocation();
	OutMaxX = Location.X + Height;
	OutMinX = Location.X - Height;
	OutMaxY = Location.Y + Width;
	OutMinY = Location.Y - Width;
}

void ATableBase::StartEvent(FEventInfo EventInfo, const FOnEventProcessed Callback)
{
	Cast<UEventWidget>(TestEventWindow->GetUserWidgetObject())->DisplayEvent(EventInfo);
	//Callback.ExecuteIfBound(TESTDeterminedValue);	

	// Uncomment below then test is over
	if (Dice)
	{
		Dice->Reset();
		Dice->OnValueDetermined.BindUObject(this, &ATableBase::OnValueDetermined, EventInfo, Callback);		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dice is invalid"));
	}
}

void ATableBase::OnValueDetermined(int32 Value, FEventInfo EventInfo, const FOnEventProcessed Callback)
{
	UE_LOG(LogTemp, Warning, TEXT("DeterminedValue: %d"), Value);		
	
	Callback.ExecuteIfBound(Value);	
}

// wtf is this. Why is it here
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

// wtf is this. Why is it here
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
