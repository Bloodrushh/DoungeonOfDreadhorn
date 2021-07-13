// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonSettings.h"

UDungeonSettings* UDungeonSettings::GetDungeonSettings()
{
	return StaticClass()->GetDefaultObject<UDungeonSettings>();
}
