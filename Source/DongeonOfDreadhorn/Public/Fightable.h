// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Fightable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFightable : public UInterface
{
	GENERATED_BODY()
};

class DONGEONOFDREADHORN_API IFightable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:	
	virtual int32 GetInitiative();
	
};
