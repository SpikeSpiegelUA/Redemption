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
		SetSkillsProgress(ECharacterSkills::MELEE, PlayerCharacter->GetSkillsProgress(ECharacterSkills::MELEE));
		SetSkillsProgress(ECharacterSkills::RANGE, PlayerCharacter->GetSkillsProgress(ECharacterSkills::RANGE));
		SetSkillsProgress(ECharacterSkills::ASSAULTSPELLS, PlayerCharacter->GetSkillsProgress(ECharacterSkills::ASSAULTSPELLS));
		SetSkillsProgress(ECharacterSkills::DEBUFFSPELLS, PlayerCharacter->GetSkillsProgress(ECharacterSkills::DEBUFFSPELLS));
		SetSkillsProgress(ECharacterSkills::RESTORATIONSPELLS, PlayerCharacter->GetSkillsProgress(ECharacterSkills::RESTORATIONSPELLS));
		SetSkillsProgress(ECharacterSkills::BUFFSPELLS, PlayerCharacter->GetSkillsProgress(ECharacterSkills::BUFFSPELLS));
		SetSkillsProgress(ECharacterSkills::DEFEND, PlayerCharacter->GetSkillsProgress(ECharacterSkills::DEFEND));
		SetSkillsProgress(ECharacterSkills::PERSUASION, PlayerCharacter->GetSkillsProgress(ECharacterSkills::PERSUASION));
		CharacterPortrait = PlayerCharacter->CharacterPortrait;
		CharacterName = FName(*PlayerCharacter->CharacterName);
		Level = PlayerCharacter->Level;
		CurrentExperience = PlayerCharacter->CurrentExperience;
		ActivatedPerks = PlayerCharacter->ActivatedPerks;
		NumberOfPerkPoints = PlayerCharacter->NumberOfPerkPoints;
		PerksCategoryNames = PlayerCharacter->GetPerksCategoryNames();
		AvailablePerks = PlayerCharacter->GetAvailablePerks();
		if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget)) {
				if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedMelee)) {
					MeleeAttackValue = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedMelee->GetAttackValue();
					MeleeWeaponElements = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedMelee->GetElementsAndTheirPercentagesStructs();
					MeleePhysicalType = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedMelee->GetPhysicalType();
				}
				else {
					MeleeAttackValue = 5;
					MeleePhysicalType = EPhysicalType::CRUSHING;
				}
				if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedRange)) {
					RangeAmmo = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedRange->GetAmmo();
					RangeAttackValue = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedRange->GetAttackValue();
					RangeWeaponElements = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedRange->GetElementsAndTheirPercentagesStructs();
					RangePhysicalType = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedRange->GetPhysicalType();
				}
				else {
					RangeAmmo = 0;
					RangeAttackValue = 0;
				}
				if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead))
					ArmorValue += UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetArmorValue();
				if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse))
					ArmorValue += UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetArmorValue();
				if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand))
					ArmorValue += UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetArmorValue();
				if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor))
					ArmorValue += UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetArmorValue();
			}
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
		LevelingUpCounter = PlayerCharacter->LevelingUpCounter;
	}
}

// Called every frame
void ACombatPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatPlayerCharacter::GetElementalResistancesFromEquipedItems()
{
	if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem) && IsValid(UIManagerWorldSubsystem->InventoryMenuWidget)) {
		for (int i = 0; i < ElementalResistances.Num(); i++) {
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetElementsAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetElementsAndTheirPercentagesStructs()[g].Element == ElementalResistances[i].Element) {
						ElementalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetElementsAndTheirPercentagesStructs()[g].Percent + ElementalResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetElementsAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetElementsAndTheirPercentagesStructs()[g].Element == ElementalResistances[i].Element) {
						ElementalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetElementsAndTheirPercentagesStructs()[g].Percent + ElementalResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetElementsAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetElementsAndTheirPercentagesStructs()[g].Element == ElementalResistances[i].Element) {
						ElementalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetElementsAndTheirPercentagesStructs()[g].Percent + ElementalResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetElementsAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetElementsAndTheirPercentagesStructs()[g].Element == ElementalResistances[i].Element) {
						ElementalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetElementsAndTheirPercentagesStructs()[g].Percent + ElementalResistances[i].Percent;
					}
		}
	}
}

void ACombatPlayerCharacter::GetPhysicalResistancesFromEquipedItems()
{
	if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem) && IsValid(UIManagerWorldSubsystem->InventoryMenuWidget)) {
		for (int i = 0; i < PhysicalResistances.Num(); i++) {
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PhysicalResistances[i].PhysicalType) {
						PhysicalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PhysicalResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PhysicalResistances[i].PhysicalType) {
						PhysicalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PhysicalResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PhysicalResistances[i].PhysicalType) {
						PhysicalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PhysicalResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PhysicalResistances[i].PhysicalType) {
						PhysicalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PhysicalResistances[i].Percent;
					}
		}
	}
}

ACombatPlayerCharacter& ACombatPlayerCharacter::operator=(const APlayerCharacter& PlayerCharacter)
{
	return *this;
}
