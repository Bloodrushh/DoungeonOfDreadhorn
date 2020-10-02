// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TableBase.h"
#include "DoungeonChunkBase.h"
#include "DungeonChunkBoardBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardBase.generated.h"

UCLASS()
class DONGEONOFDREADHORN_API ABoardBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoardBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<ADungeonChunkBoardBase*> BoardChunks;
	
	void OnDungeonGenerated(TArray<ADoungeonChunkBase*> Chunks);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<ADungeonChunkBoardBase> DungeonChunkBoardClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ATableBase* Table;

	void OnMapGenerated();
};
