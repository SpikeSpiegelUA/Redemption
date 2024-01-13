#include "CombatPlayerCharacter.h"
#include "..\Characters\AI Controllers\Combat\CombatPlayerCharacterAIController.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Characters\Animation\Combat\CombatPlayerCharacterAnimInstanc.h"

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
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee)) {
				MeleeAttackValue = PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee->GetAttackValue();
				MeleeWeaponElements = PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee->GetElementsAndTheirPercentagesStructs();
			}
			else
				MeleeAttackValue = 10;
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedRange)) {
				RangeAmmo = PlayerCharacter->GetInventoryMenuWidget()->EquipedRange->GetAmmo();
				RangeAttackValue = PlayerCharacter->GetInventoryMenuWidget()->EquipedRange->GetAttackValue();
				RangeWeaponElements = PlayerCharacter->GetInventoryMenuWidget()->EquipedRange->GetElementsAndTheirPercentagesStructs();
			} 
			else {
				RangeAmmo = 0;
				RangeAttackValue = 0;
			}
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedHead))
				ArmorValue += PlayerCharacter->GetInventoryMenuWidget()->EquipedHead->GetArmorValue();
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse))
				ArmorValue += PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse->GetArmorValue();
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedHand))
				ArmorValue += PlayerCharacter->GetInventoryMenuWidget()->EquipedHand->GetArmorValue();
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor))
				ArmorValue += PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor->GetArmorValue();
		}
		GetResistancesFromEquipedItems();
		AvailableSpells = PlayerCharacter->GetAvailableSkills();
		CurrentHP = PlayerCharacter->CurrentHP;
		MaxHP = PlayerCharacter->MaxHP;
		CurrentMana = PlayerCharacter->CurrentMana;
		MaxMana = PlayerCharacter->MaxMana;
	}
}

// Called every frame
void ACombatPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatPlayerCharacter::GetResistancesFromEquipedItems()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetInventoryMenuWidget())) {
			for (FElementAndItsPercentageStruct ResistancesArrayWhereToAddElementPercentage : ElementalResistances) {

			}
			for (int i = 0; i < ElementalResistances.Num(); i++) {
				if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedHead))
					for(int g = 0; g < PlayerCharacter->GetInventoryMenuWidget()->EquipedHead->GetElementsAndTheirPercentagesStructs().Num(); g++)
						if (PlayerCharacter->GetInventoryMenuWidget()->EquipedHead->GetElementsAndTheirPercentagesStructs()[g].Element == ElementalResistances[i].Element) {
							ElementalResistances[i].Percent = PlayerCharacter->GetInventoryMenuWidget()->EquipedHead->GetElementsAndTheirPercentagesStructs()[g].Percent + ElementalResistances[i].Percent;
						}
				if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse))
					for (int g = 0; g < PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse->GetElementsAndTheirPercentagesStructs().Num(); g++)
						if (PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse->GetElementsAndTheirPercentagesStructs()[g].Element == ElementalResistances[i].Element) {
							ElementalResistances[i].Percent = PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse->GetElementsAndTheirPercentagesStructs()[g].Percent + ElementalResistances[i].Percent;
						}
				if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedHand))
					for (int g = 0; g < PlayerCharacter->GetInventoryMenuWidget()->EquipedHand->GetElementsAndTheirPercentagesStructs().Num(); g++)
						if (PlayerCharacter->GetInventoryMenuWidget()->EquipedHand->GetElementsAndTheirPercentagesStructs()[g].Element == ElementalResistances[i].Element) {
							ElementalResistances[i].Percent = PlayerCharacter->GetInventoryMenuWidget()->EquipedHand->GetElementsAndTheirPercentagesStructs()[g].Percent + ElementalResistances[i].Percent;
						}
				if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor))
					for (int g = 0; g < PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor->GetElementsAndTheirPercentagesStructs().Num(); g++)
						if (PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor->GetElementsAndTheirPercentagesStructs()[g].Element == ElementalResistances[i].Element) {
							ElementalResistances[i].Percent = PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor->GetElementsAndTheirPercentagesStructs()[g].Percent + ElementalResistances[i].Percent;
						}
			}
	}
}

ACombatPlayerCharacter& ACombatPlayerCharacter::operator=(const APlayerCharacter& PlayerCharacter)
{
	return *this;
}
