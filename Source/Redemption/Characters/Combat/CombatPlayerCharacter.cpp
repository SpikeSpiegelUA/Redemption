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
		SetStat(ECharacterStats::STRENGTH, PlayerCharacter->GetStat(ECharacterStats::STRENGTH));
		SetStat(ECharacterStats::PERCEPTION, PlayerCharacter->GetStat(ECharacterStats::PERCEPTION));
		SetStat(ECharacterStats::ENDURANCE, PlayerCharacter->GetStat(ECharacterStats::ENDURANCE));
		SetStat(ECharacterStats::CHARISMA, PlayerCharacter->GetStat(ECharacterStats::CHARISMA));
		SetStat(ECharacterStats::INTELLIGENCE, PlayerCharacter->GetStat(ECharacterStats::INTELLIGENCE));
		SetStat(ECharacterStats::WILL, PlayerCharacter->GetStat(ECharacterStats::WILL));
		SetStat(ECharacterStats::AGILITY, PlayerCharacter->GetStat(ECharacterStats::AGILITY));
		SetStat(ECharacterStats::LUCK, PlayerCharacter->GetStat(ECharacterStats::LUCK));
		SetSkill(ECharacterSkills::MELEE, PlayerCharacter->GetSkill(ECharacterSkills::MELEE));
		SetSkill(ECharacterSkills::RANGE, PlayerCharacter->GetSkill(ECharacterSkills::RANGE));
		SetSkill(ECharacterSkills::ASSAULTSPELLS, PlayerCharacter->GetSkill(ECharacterSkills::ASSAULTSPELLS));
		SetSkill(ECharacterSkills::DEBUFFSPELLS, PlayerCharacter->GetSkill(ECharacterSkills::DEBUFFSPELLS));
		SetSkill(ECharacterSkills::RESTORATIONSPELLS, PlayerCharacter->GetSkill(ECharacterSkills::RESTORATIONSPELLS));
		SetSkill(ECharacterSkills::BUFFSPELLS, PlayerCharacter->GetSkill(ECharacterSkills::BUFFSPELLS));
		SetSkill(ECharacterSkills::DEFEND, PlayerCharacter->GetSkill(ECharacterSkills::DEFEND));
		SetSkill(ECharacterSkills::PERSUASION, PlayerCharacter->GetSkill(ECharacterSkills::PERSUASION));
		Level = PlayerCharacter->Level;
		if (IsValid(PlayerCharacter->GetInventoryMenuWidget())) {
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee)) {
				MeleeAttackValue = PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee->GetAttackValue();
				MeleeWeaponElements = PlayerCharacter->GetInventoryMenuWidget()->EquipedMelee->GetElementsAndTheirPercentagesStructs();
			}
			else
				MeleeAttackValue = 5;
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
		GetElementalResistancesFromEquipedItems();
		GetPhysicalResistancesFromEquipedItems();
		AvailableSpells = PlayerCharacter->GetAvailableSkills();
		CurrentHP = PlayerCharacter->CurrentHP;
		MaxHP = PlayerCharacter->MaxHP;
		CurrentMana = PlayerCharacter->CurrentMana;
		MaxMana = PlayerCharacter->MaxMana;
		EvasionChance = PlayerCharacter->EvasionChance;
		TargetingChance = PlayerCharacter->TargetingChance;
	}
}

// Called every frame
void ACombatPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatPlayerCharacter::GetElementalResistancesFromEquipedItems()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetInventoryMenuWidget())) {
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

void ACombatPlayerCharacter::GetPhysicalResistancesFromEquipedItems()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter) && IsValid(PlayerCharacter->GetInventoryMenuWidget())) {
		for (int i = 0; i < PhysicalResistances.Num(); i++) {
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedHead))
				for (int g = 0; g < PlayerCharacter->GetInventoryMenuWidget()->EquipedHead->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (PlayerCharacter->GetInventoryMenuWidget()->EquipedHead->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PhysicalResistances[i].PhysicalType) {
						PhysicalResistances[i].Percent = PlayerCharacter->GetInventoryMenuWidget()->EquipedHead->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PhysicalResistances[i].Percent;
					}
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse))
				for (int g = 0; g < PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PhysicalResistances[i].PhysicalType) {
						PhysicalResistances[i].Percent = PlayerCharacter->GetInventoryMenuWidget()->EquipedTorse->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PhysicalResistances[i].Percent;
					}
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedHand))
				for (int g = 0; g < PlayerCharacter->GetInventoryMenuWidget()->EquipedHand->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (PlayerCharacter->GetInventoryMenuWidget()->EquipedHand->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PhysicalResistances[i].PhysicalType) {
						PhysicalResistances[i].Percent = PlayerCharacter->GetInventoryMenuWidget()->EquipedHand->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PhysicalResistances[i].Percent;
					}
			if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor))
				for (int g = 0; g < PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PhysicalResistances[i].PhysicalType) {
						PhysicalResistances[i].Percent = PlayerCharacter->GetInventoryMenuWidget()->EquipedLowerArmor->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PhysicalResistances[i].Percent;
					}
		}
	}
}

ACombatPlayerCharacter& ACombatPlayerCharacter::operator=(const APlayerCharacter& PlayerCharacter)
{
	return *this;
}
