// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ProjectTypes.h"
#include "Engine/DeveloperSettings.h"
#include "DungeonSettings.generated.h"

UCLASS(config = DungeonSettings, defaultconfig)
class DONGEONOFDREADHORN_API UDungeonSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	virtual FName GetCategoryName() const override { return FName("Game"); }

	UFUNCTION(BlueprintPure, Category = "Bank settings")
	static UDungeonSettings* GetDungeonSettings();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config)
	TMap<ETier, FDungeonConfiguration> Configurations;
};
