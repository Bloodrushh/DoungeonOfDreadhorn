
#pragma once
#include "CoreMinimal.h"
#include "ProjectTypes.generated.h"


UENUM(BlueprintType)
enum class EEventType : uint8
{
	Trap,
	Fight,
	Chest
};

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	Strength,
	Agility,
	Intelligence
};