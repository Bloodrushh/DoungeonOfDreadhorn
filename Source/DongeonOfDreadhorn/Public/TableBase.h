// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectTypes.h"
#include "DODPlayerController.h"
#include "DiceBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TableBase.generated.h"

UCLASS()
class DONGEONOFDREADHORN_API ATableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		float Height = 500.0f;

	UPROPERTY(EditDefaultsOnly)
		float Width = 500.0f;

	void GetBounce(float& OutMaxX, float& OutMinX, float& OutMaxY, float& OutMinY);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ADiceBase* Dice;

	UFUNCTION(BlueprintCallable)
	void StartEvent(FEventInfo EventInfo, const FOnEventProcessed Callback);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ADODPlayerController* DODPlayerController;
	//VisibleAnywhere
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		APlayerPawn* PlayerPawn;

	UFUNCTION()
	void OnValueDetermined(int32 Value, FEventInfo EventInfo, const FOnEventProcessed Callback);

	UPROPERTY(EditDefaultsOnly)
		UDataTable* CharactersDT;

	UPROPERTY(EditDefaultsOnly)
		UDataTable* CharactersNamesDT;

	UPROPERTY(EditDefaultsOnly)
		FName HumanRowName;

	UPROPERTY(EditDefaultsOnly)
		FName ElvenRowName;

	UPROPERTY(EditDefaultsOnly)
		FName DwarvenRowName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FCharacterInfo> Characters;

	void GenerateCharactersPool();

	UPROPERTY(EditDefaultsOnly)
		int32 CharactersToGenerate = 5;

	//FOnEvenProcessed

	bool GetRandomName(ERace Race, EGender Gender, FText& OutFirstName, FText& OutSecondName);

	UPROPERTY(EditAnywhere)
		int32 TESTDeterminedValue = 0;
	
};

