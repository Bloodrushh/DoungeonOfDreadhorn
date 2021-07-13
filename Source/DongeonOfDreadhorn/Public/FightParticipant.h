// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FightParticipant.generated.h"


UCLASS()
class DONGEONOFDREADHORN_API AFightParticipant : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFightParticipant();

public:	
	UFUNCTION(BlueprintCallable)
	virtual int32 GetInitiative();

	UFUNCTION(BlueprintCallable)
	virtual void PerformMove();

	UFUNCTION(BlueprintCallable)
	virtual void OnMovePerformed();
	
};
