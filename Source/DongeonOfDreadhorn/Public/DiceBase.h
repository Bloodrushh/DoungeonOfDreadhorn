// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DiceFaceComponent.h"
#include "PickupableInterface.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiceBase.generated.h"

DECLARE_DELEGATE_OneParam(FOnValueDetermined, int32 /*Value*/);

UCLASS()
class DONGEONOFDREADHORN_API ADiceBase : public AActor, public IPickupableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADiceBase(); //const FObjectInitializer& ObjectInitializer

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

	virtual void OnPickup(USceneComponent* AttachTo) override;
	virtual void OnDrop() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bThrown = false;
	bool bValueDetermined = false;

	void DetermineValue(UDiceFaceComponent*& OutDiceFace);

	TArray<UDiceFaceComponent*> DiceFaces;

	void Reset();

	FOnValueDetermined OnValueDetermined;

	FTransform InitialTransform;

	
};
