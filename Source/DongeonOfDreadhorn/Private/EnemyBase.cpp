// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "PlayerPawn.h"
#include "DoungeonConfigurationSubsystem.h"
#include "FightManager.h"
#include "Kismet/GameplayStatics.h"

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
	FightManager->OnParticipantDied(this);
	void OnDiedBP();
}

void AEnemyBase::TakeDamage(int32 Amount, EAttack Attack)
{		
	OnDamageTakenBP(Amount, Attack);
	Health -= Amount;
	if (Health <= 0)
	{
		Die();
	}
}

bool AEnemyBase::CanTakeDamage()
{	
	return false;
}

void AEnemyBase::PerformMove()
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyBase::PerformMove called"));
	// TODO Should randomise move type
	EMoveType Move =  EMoveType::Attack;

	switch (Move)
	{
	case EMoveType::None:
		break;
	case EMoveType::Attack:
		ToggleAttackNotifaction(true);
		StartAttack_Delayed();
		break;
	case EMoveType::Buff:
		break;
	default:
		break;		
	}	
}

void AEnemyBase::OnMovePerformed()
{
	FightManager->OnMovePerformed(this);
}

void AEnemyBase::FinishAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyBase::FinishAttack called"));
	ToggleAttackNotifaction(false);
	
	if(!PlayerPawn){ return; }
	
	PlayerPawn->TakeDamage(Damage, AttackType);	
	OnMovePerformed();
}

void AEnemyBase::StartAttack_Delayed()
{
	float DelayBeforeAttack = GetGameInstance()->GetSubsystem<UDoungeonConfigurationSubsystem>()->Configuration.DelayBeforeEnemyAttack;
	UE_LOG(LogTemp, Warning, TEXT("AEnemyBase::StartAttack_Delayed called. Delat: %f"), DelayBeforeAttack);

	GetWorldTimerManager().SetTimer(PerformAttackTimerHandle, this, &AEnemyBase::StartAttack, DelayBeforeAttack);
}

int32 AEnemyBase::GetInitiative()
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemyBase::GetInitiative: %d"), Initiative);
	return Initiative;
}

