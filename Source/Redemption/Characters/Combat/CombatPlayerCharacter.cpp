#include "CombatPlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\Combat\CombatPlayerCharacterAIController.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\Combat\CombatPlayerCharacterAnimInstanc.h"

// Sets default values
ACombatPlayerCharacter::ACombatPlayerCharacter()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACombatPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		Strength = PlayerCharacter->Strength;
		Perception = PlayerCharacter->Perception;
		Endurance = PlayerCharacter->Endurance;
		Charisma = PlayerCharacter->Charisma;
		Intelligence = PlayerCharacter->Intelligence;
		Will = PlayerCharacter->Will;
		Agility = PlayerCharacter->Agility;
		Luck = PlayerCharacter->Luck;
		if (IsValid(PlayerCharacter->GetInventoryMenuWidget())) {
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee))
				MeleeAttackValue = PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee->GetAttackValue();
			else
				MeleeAttackValue = 10;
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedRange))
				RangeAttackValue = PlayerCharacter->GetInventoryMenuWidget()->EquipedRange->GetAttackValue();
			else
				RangeAttackValue = 0;
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedHead))
				ArmorValue += PlayerCharacter->GetInventoryMenuWidget()->EquipedHead->GetArmorValue();
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse))
				ArmorValue += PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse->GetArmorValue();
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedHand))
				ArmorValue += PlayerCharacter->GetInventoryMenuWidget()->EquipedHand->GetArmorValue();
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor))
				ArmorValue += PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor->GetArmorValue();
		}
	}
}

// Called every frame
void ACombatPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
