// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/TextRenderComponent.h"
#include "Components/ArrowComponent.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DiceFaceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DONGEONOFDREADHORN_API UDiceFaceComponent : public UArrowComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDiceFaceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditDefaultsOnly)
		int32 FaceValue = 0;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UTextRenderComponent* ValueText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UArrowComponent* Direction;*/
};
