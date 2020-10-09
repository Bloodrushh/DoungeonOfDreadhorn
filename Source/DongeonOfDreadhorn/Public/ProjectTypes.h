
#pragma once
#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "ProjectTypes.generated.h"


UENUM(BlueprintType)
enum class EEvent : uint8
{
	Trap,
	Fight,
	Chest
};

UENUM(BlueprintType)
enum class EAttribute : uint8
{
	Health,
	Damage,
	ActionPoints,
	Initiative,
	Strength,
	Agility,
	Intelligence
};

UENUM(BlueprintType)
enum class ECharacterClass: uint8
{
	Rogue,
	Warrior,
	Magician
};

UENUM(BlueprintType)
enum class ERace: uint8
{
	Human,
	Elf,
	Dwarf
};

UENUM(BlueprintType)
enum class ELanguage: uint8
{
	Human,
	Elven,
	Dwarven
};

UENUM(BlueprintType)
enum class ETier: uint8
{
	Common,
	Rare,
	Legendary,
	Epic	
};

UENUM(BlueprintType)
enum class EAttack : uint8
{
	Dexterous,
	Power,
	Magical,
};

UENUM(BlueprintType)
enum class EGender : uint8
{
	Male,
	Female
};


UENUM(BlueprintType)
enum class EEffect : uint8
{
	Null,
	Add,
	Subtract,
	Multiply,
	Divide
};

USTRUCT(BlueprintType)
struct FAttributesInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly ,BlueprintReadOnly)
		TMap<EAttribute, int32> Attributes;
	
	FAttributesInfo()
	{
		Attributes.Add(EAttribute::Health, 100);
		Attributes.Add(EAttribute::Damage, 10);
		Attributes.Add(EAttribute::ActionPoints, 4);
		Attributes.Add(EAttribute::Initiative, 30);
		Attributes.Add(EAttribute::Strength, 1);
		Attributes.Add(EAttribute::Agility, 0);
		Attributes.Add(EAttribute::Intelligence, 0);
	}	
};

USTRUCT(BlueprintType)
struct FGeneralInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
		FText FirstName;

	UPROPERTY(BlueprintReadOnly)
		FText SecondName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EGender Gender;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ERace Race;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<ELanguage> Languages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterClass Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ETier Tier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EAttack Attack;	
};

USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FGeneralInfo GeneralInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FAttributesInfo AttributesInfo;
};

USTRUCT(BlueprintType)
struct FChacterNames : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FText> MaleFirstNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FText> FemaleFirstNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FText> MaleSecondNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FText> FemaleSecondNames;
};

USTRUCT(BlueprintType)
struct FEventInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EEvent Event;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bAttributeBased;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EAttribute Attribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bReversed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<int32> SuccessNumbers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EEffect SuccessEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EAttribute SuccessAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 SuccessAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EEffect FailEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EAttribute FailAttribute;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 FailAmount;	
};