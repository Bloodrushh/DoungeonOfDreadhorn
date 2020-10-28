// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include  "ProjectTypes.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EventWidget.generated.h"

UCLASS(Blueprintable)
class DONGEONOFDREADHORN_API UEventWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayEvent(FEventInfo EventInfo);
	
};
