
#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ProjectTypes.generated.h"

class AFightParticipant;
class AEventTriggerBase;
class ADoungeonChunkBase;
class APlayerPawn;
class AEnemyBase;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEventProcessed, int32, DeterminedValue);
DECLARE_DELEGATE_OneParam(FOnValueDetermined, int32 /*Value*/);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovePerformed);

UENUM(BlueprintType)
enum class EEvent : uint8 { Trap, Fight, Chest };

UENUM(BlueprintType)
enum class EAttribute : uint8 {
  Health,
  Damage,
  ActionPoints,
  Initiative,
  Strength,
  Agility,
  Intelligence
};

UENUM(BlueprintType)
enum class ECharacterClass : uint8 { Rogue, Warrior, Magician };

UENUM(BlueprintType)
enum class ERace : uint8 { Human, Elf, Dwarf };

UENUM(BlueprintType)
enum class ELanguage : uint8 { Human, Elven, Dwarven };

UENUM(BlueprintType)
enum class ETier : uint8 { Common, Rare, Legendary, Epic };

UENUM(BlueprintType)
enum class EAttack : uint8 {
  Dexterous,
  Power,
  Magical,
};

UENUM(BlueprintType)
enum class EGender : uint8 { Male, Female };

UENUM(BlueprintType)
enum class EEffect : uint8 { None, Add, Subtract, Multiply, Divide };

UENUM(BlueprintType)
enum class EMoveType : uint8 { None, Attack, Buff };

USTRUCT(BlueprintType)
struct FAttributesInfo {
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TMap<EAttribute, int32> Attributes;

  FAttributesInfo() {
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
struct FGeneralInfo {
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

  FGeneralInfo() { Languages.SetNumZeroed(1); }
};

USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase {
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FGeneralInfo GeneralInfo;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FAttributesInfo AttributesInfo;
};

USTRUCT(BlueprintType)
struct FChacterNames : public FTableRowBase {
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
struct FCondition {
  GENERATED_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  FText Description;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
  TArray<FText> KeyWordsValues;

  /*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
          EEffect Effect;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
          EAttribute Attribute;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
          int32 Amount;*/
};

USTRUCT(BlueprintType)
struct FEventInfo
{
	  GENERATED_BODY()

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  FText Description;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  UTexture2D *Icon;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  EEvent Event;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  bool bAttributeBased;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  bool bGroup;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  EAttribute Attribute;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  int32 AttributeThreshold;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  int32 BonusValue;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  bool bReversed;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  TArray<int32> SuccessNumbers;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  bool bHardCondition;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  TArray<FCondition> SuccessConditions;

	  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	  TArray<FCondition> FailureConditions;
};

USTRUCT(BlueprintType)
struct FDungeonConfiguration
{
	GENERATED_BODY()

	// dungeon length(chunks amount to spawn)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Length;

	// used to determine amount of triggers to spawn by formula: Length - entrance - exit / EventTriggersStep.
	// example: 95 / 10 = 9. This means that triggers will spawn every 9 chunks
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 EventTriggersSpawnAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSoftClassPtr<ADoungeonChunkBase>> ChunkClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftClassPtr<ADoungeonChunkBase> EntranceChunkClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftClassPtr<ADoungeonChunkBase> ExitChunkClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DelayBeforeEnemyAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSoftClassPtr<AEventTriggerBase>> EventTriggerClasses;	
};

USTRUCT(BlueprintType)
struct FFightInitiateInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<TSoftClassPtr<AFightParticipant>> EnemyClasses;	
};
