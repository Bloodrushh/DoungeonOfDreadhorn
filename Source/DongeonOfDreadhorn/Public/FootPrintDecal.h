// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FootPrintDecal.generated.h"

class UDecalComponent;
class UMaterialInstance;

UCLASS()
class DONGEONOFDREADHORN_API AFootPrintDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFootPrintDecal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UDecalComponent* DecalComponent;
	
	UPROPERTY(EditDefaultsOnly)
		UMaterialInstance* DecalMaterial;
};
