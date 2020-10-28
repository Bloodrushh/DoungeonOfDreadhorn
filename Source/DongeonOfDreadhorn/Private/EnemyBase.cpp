// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "EventTriggerFight.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::Die()
{
	//AEventTriggerFight* EventTriggerFight = Cast<AEventTriggerFight>(EventTrigger);
	if (AEventTriggerFight* EventTriggerFight = Cast<AEventTriggerFight>(EventTrigger)) { EventTriggerFight->OnEnemyDied(this); }
	void OnDiedBP();
}

void AEnemyBase::TakeDamage(int32 Amount, EAttack Attack)
{
	OnDamageTakenBP(Amount, Attack);
}

bool AEnemyBase::CanTakeDamage()
{	
	return false;
}

