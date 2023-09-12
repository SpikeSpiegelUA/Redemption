#include "CombatAllyNPC.h"

// Sets default values
ACombatAllyNPC::ACombatAllyNPC()
{
	// Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACombatAllyNPC::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACombatAllyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACombatAllyNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

