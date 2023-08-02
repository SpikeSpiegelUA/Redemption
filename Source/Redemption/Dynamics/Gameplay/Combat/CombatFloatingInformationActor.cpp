// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatFloatingInformationActor.h"

// Sets default values
ACombatFloatingInformationActor::ACombatFloatingInformationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Create floating widget
	CombatFloatingInformationComponentWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CombatFloatingInformation"));
	CombatFloatingInformationComponentWidget->SetWidgetSpace(EWidgetSpace::Screen);
	CombatFloatingInformationComponentWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACombatFloatingInformationActor::BeginPlay()
{
	Super::BeginPlay();
	
	//Set up properties for CombatFloatingInformationWidget
	CombatFloatingInformationComponentWidget->SetWidgetClass(CombatFloatingInformationClass);
	CombatFloatingInformationWidget = Cast<UCombatFloatingInformation>(CombatFloatingInformationComponentWidget->GetWidget());
	if (IsValid(CombatFloatingInformationWidget)) {
		CombatFloatingInformationWidget->SetMainTextBlockText(CombatFloatingInformationText);
		TArray<UStaticMeshComponent*> Components;
		this->GetComponents<UStaticMeshComponent>(Components);
		for (int32 i = 0; i < Components.Num(); i++)
		{
			UStaticMeshComponent* StaticMeshComponent = Components[i];
			StaticMeshComponent->AddImpulse(FVector(FMath::FRandRange(-400.0, 400.0), FMath::FRandRange(-400.0, 400.0), FMath::FRandRange(400.0, 800.0)), NAME_None, true);
		}
	}
}

// Called every frame
void ACombatFloatingInformationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatFloatingInformationActor::SetCombatFloatingInformationText(FText& NewCombatFloatingInformationText)
{
	CombatFloatingInformationText = NewCombatFloatingInformationText;
}

