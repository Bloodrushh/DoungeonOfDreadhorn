// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProjectTypes.h"
#include "DODPlayerController.h"
#include "DiceBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TableBase.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEvenProcessed, bool, bSucceed);

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
	void StartEvent(EEventType Event, EAttributeType Attribute, const FOnEvenProcessed Callback);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ADODPlayerController* DODPlayerController;

	UFUNCTION()
	void OnValueDetermined(int32 Value, EEventType Event, EAttributeType Attribute, const FOnEvenProcessed Callback);
};
