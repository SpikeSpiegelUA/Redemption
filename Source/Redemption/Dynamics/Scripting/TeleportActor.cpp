// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportActor.h"
#include "..\Characters\Player\PlayerCharacter.h"

// Sets default values
ATeleportActor::ATeleportActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(RootComponent);
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ATeleportActor::OnActorOverlap);
}

// Called when the game starts or when spawned
void ATeleportActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATeleportActor::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult) {
	if (IsValid(OtherActor) && OtherActor->IsA(APlayerCharacter::StaticClass())) {
		OtherActor->SetActorLocation(TeleportActor->GetActorLocation());
	}
}

// Called every frame
void ATeleportActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

