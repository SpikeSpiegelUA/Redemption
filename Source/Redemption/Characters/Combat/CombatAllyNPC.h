#pragma once

#include "CoreMinimal.h"
#include "CombatAllies.h"
#include "CombatAllyNPC.generated.h"

UCLASS()
class REDEMPTION_API ACombatAllyNPC : public ACombatAllies
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACombatAllyNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
