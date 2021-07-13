// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "ProjectTypes.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "DamageableInterface.h"
#include "EventTriggerBase.h"
#include "FightParticipant.h"

#include "EnemyBase.generated.h"

class AFightManager;
UCLASS()
class DONGEONOFDREADHORN_API AEnemyBase : public AFightParticipant
    , public IDamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Die();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDiedBP();

	void TakeDamage(int32 Amount, EAttack Attack) override;

	bool CanTakeDamage() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTakenBP(int32 Amount, EAttack Attack);

	virtual void PerformMove() override;

	virtual void OnMovePerformed() override;

	UFUNCTION(BlueprintCallable)
	void FinishAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void StartAttack();

	void StartAttack_Delayed();

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleAttackNotifaction(bool bEnable);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxCollision;	

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	APlayerPawn* PlayerPawn;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	AFightManager* FightManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAttack AttackType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Initiative;

	FTimerHandle PerformAttackTimerHandle;

	virtual int32 GetInitiative() override;
};
