// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatFloatingInformationActor.h"

// Sets default values
ACombatFloatingInformationActor::ACombatFloatingInformationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACombatFloatingInformationActor::BeginPlay()
{
	Super::BeginPlay();
	
	//Set up properties for CombatFloatingInformationWidget
	UWidgetComponent* CombatFloatingInformationComponentWidget = FindComponentByClass<UWidgetComponent>();
	if(IsValid(CombatFloatingInformationComponentWidget))
		CombatFloatingInformationWidget = Cast<UCombatFloatingInformation>(CombatFloatingInformationComponentWidget->GetWidget());
	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if(IsValid(StaticMeshComponent))
		StaticMeshComponent->AddImpulse(FVector(FMath::FRandRange(-200.0, 200.0), FMath::FRandRange(-200.0, 200.0), FMath::FRandRange(-200.0, 200.0)), NAME_None, true);
	GetWorld()->GetTimerManager().SetTimer(ActorDestroyTimerHandle, this, &ACombatFloatingInformationActor::DestroyThisActor, 2.f, false);
}

// Called every frame
void ACombatFloatingInformationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatFloatingInformationActor::DestroyThisActor()
{
	this->Destroy();
}

void ACombatFloatingInformationActor::SetCombatFloatingInformationText(const FText& NewCombatFloatingInformationText)
{
	CombatFloatingInformationWidget->SetMainTextBlockText(NewCombatFloatingInformationText);
}

