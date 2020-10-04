// Fill out your copyright notice in the Description page of Project Settings.


#include "DiceFaceComponent.h"

// Sets default values for this component's properties
UDiceFaceComponent::UDiceFaceComponent()
{	
	PrimaryComponentTick.bCanEverTick = false;	
	
	/*ValueText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ValueText"));
	Direction = CreateDefaultSubobject<UArrowComponent>(TEXT("Direction"));
	ValueText->SetText(FString::FromInt(FaceValue));*/
}


// Called when the game starts
void UDiceFaceComponent::BeginPlay()
{
	Super::BeginPlay();	
}
