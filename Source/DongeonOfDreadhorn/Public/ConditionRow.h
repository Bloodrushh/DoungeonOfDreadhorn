// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ProjectTypes.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConditionRow.generated.h"

/**
 * 
 */
UCLASS()
class DONGEONOFDREADHORN_API UConditionRow : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		FCondition Condition;	
};
