// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DoungeonChunkBase.generated.h"

UCLASS()
class DONGEONOFDREADHORN_API ADoungeonChunkBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoungeonChunkBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY()
		TArray<USceneComponent*> PossibleExits;

	UPROPERTY()
		TArray<USceneComponent*> ValidExits;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADoungeonChunkBase> DeadEndChunkClass;

	UPROPERTY()
		int32 ValidExitIndex = -1;

	UFUNCTION()
		void ChooseValidExit();

	UFUNCTION(BlueprintCallable)
		bool TryGetValidExitSnapTransform( FTransform& OutTransform);

	UFUNCTION()
		void FindAndChachePossibleExits();

	UPROPERTY()
		FName ExitSnapPointTag = TEXT("ExitSnapPoint");

	UPROPERTY()
		TArray<USceneComponent*> Floors;

	UFUNCTION()
		void FindAndChacheFloors();

	UPROPERTY()
		FName FloorTag = TEXT("FloorSnapPoint");

	UFUNCTION(BlueprintCallable)
		void GetPossibleExits(TArray<USceneComponent*>& OutPossibleExits);

	UFUNCTION()
		void PlaceDeadEnds();

	UFUNCTION()
		void GetDeadExits(TArray<USceneComponent*>& OutExits);
};
