// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include "ProjectTypes.h"
#include "CoreMinimal.h"
#include "DamageableInterface.h"
#include "EventTriggerBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

UCLASS()
class DONGEONOFDREADHORN_API AEnemyBase : public AActor, public IDamageableInterface
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UBoxComponent* BoxCollision;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		AEventTriggerBase* EventTrigger;

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
		APlayerPawn* PlayerPawn;

	void Die();

	UFUNCTION(BlueprintImplementableEvent)
		void OnDiedBP();

	void TakeDamage(int32 Amount, EAttack Attack) override;

	bool CanTakeDamage() override;

	UFUNCTION(BlueprintImplementableEvent)
		void OnDamageTakenBP(int32 Amount, EAttack Attack);

	/*UPROPERTY(BlueprintAssignable)
	FOnEnemyDied OnDiedDelegate;*/
	
};
