#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatAllies.h"
#include "CombatPlayerCharacter.generated.h"

UCLASS()
class REDEMPTION_API ACombatPlayerCharacter : public ACombatAllies
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACombatPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetElementalResistancesFromEquipedItems();
	void GetPhysicalResistancesFromEquipedItems();

	class ACombatPlayerCharacter& operator =(const class APlayerCharacter& PlayerCharacter);
};
