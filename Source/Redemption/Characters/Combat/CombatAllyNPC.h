#pragma once

#include "CoreMinimal.h"
#include "CombatAllies.h"
#include "..\SaveSystem\Interfaces\SavableObjectInterface.h"
#include "CombatAllyNPC.generated.h"

UCLASS()
class REDEMPTION_API ACombatAllyNPC : public ACombatAllies, public ISavableObjectInterface
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	ACombatAllyNPC();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
