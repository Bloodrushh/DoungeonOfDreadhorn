// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickupableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DONGEONOFDREADHORN_API IPickupableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnPickup(USceneComponent* AttachTo);

	virtual void OnDrop();

};
