// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ProjectTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DoungeonConfigurationSubsystem.generated.h"

UCLASS()
class DONGEONOFDREADHORN_API UDoungeonConfigurationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// TODO: Save settings after set
	UFUNCTION(BlueprintCallable)
	void ChangeConfiguration(FDungeonConfiguration NewConfiguration);

public:
	UPROPERTY(BlueprintReadOnly)
	FDungeonConfiguration Configuration;
	
};
