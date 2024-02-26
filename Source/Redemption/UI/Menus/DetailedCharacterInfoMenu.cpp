// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailedCharacterInfoMenu.h"
#include <Redemption/Miscellaneous/SkillsSpellsAndEffectsActions.h>

bool UDetailedCharacterInfoMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(BackButton)) {
		BackButton->OnClicked.AddDynamic(this, &UDetailedCharacterInfoMenu::BackButtonOnClicked);
		BackButton->OnHovered.AddDynamic(this, &UDetailedCharacterInfoMenu::BackButtonOnHovered);
	}
	if (IsValid(AbilitiesButton)) {
		AbilitiesButton->OnClicked.AddDynamic(this, &UDetailedCharacterInfoMenu::AbilitiesButtonOnClicked);
		AbilitiesButton->OnHovered.AddDynamic(this, &UDetailedCharacterInfoMenu::AbilitiesButtonOnHovered);
	}
	if (IsValid(ToggleInfoButton)) {
		ToggleInfoButton->OnClicked.AddDynamic(this, &UDetailedCharacterInfoMenu::ToggleInfoButtonOnClicked);
		ToggleInfoButton->OnHovered.AddDynamic(this, &UDetailedCharacterInfoMenu::ToggleInfoButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UDetailedCharacterInfoMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDetailedCharacterInfoMenu::SetCharacterInfoForAlly()
{
	CharacterNameTextBlock->SetText(FText::FromName(Ally->GetCharacterName()));
	FString StringToSet{};
	StringToSet.Append("Level: ");
	StringToSet.AppendInt(Ally->Level);
	CharacterLevelTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("HP: ");
	StringToSet.AppendInt(Ally->CurrentHP);
	StringToSet.Append("/");
	StringToSet.AppendInt(Ally->MaxHP);
	CharacterHPTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Mana: ");
	StringToSet.AppendInt(Ally->CurrentMana);
	StringToSet.Append("/");
	StringToSet.AppendInt(Ally->MaxMana);
	CharacterManaTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Strength: ");
	StringToSet.AppendInt(Ally->GetStat(ECharacterStats::STRENGTH));
	StrengthStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Perception: ");
	StringToSet.AppendInt(Ally->GetStat(ECharacterStats::PERCEPTION));
	PerceptionStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Endurance: ");
	StringToSet.AppendInt(Ally->GetStat(ECharacterStats::ENDURANCE));
	EnduranceStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Charisma: ");
	StringToSet.AppendInt(Ally->GetStat(ECharacterStats::CHARISMA));
	CharismaStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Intelligence: ");
	StringToSet.AppendInt(Ally->GetStat(ECharacterStats::INTELLIGENCE));
	IntelligenceStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Will: ");
	StringToSet.AppendInt(Ally->GetStat(ECharacterStats::WILL));
	WillStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Agility: ");
	StringToSet.AppendInt(Ally->GetStat(ECharacterStats::AGILITY));
	AgilityStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Luck: ");
	StringToSet.AppendInt(Ally->GetStat(ECharacterStats::LUCK));
	LuckStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Melee: ");
	StringToSet.AppendInt(Ally->GetSkill(ECharacterSkills::MELEE));
	StringToSet.Append("/100");
	MeleeSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Range: ");
	StringToSet.AppendInt(Ally->GetSkill(ECharacterSkills::RANGE));
	StringToSet.Append("/100");
	RangeSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Assault spells: ");
	StringToSet.AppendInt(Ally->GetSkill(ECharacterSkills::ASSAULTSPELLS));
	StringToSet.Append("/100");
	AssaultSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Debuff spells: ");
	StringToSet.AppendInt(Ally->GetSkill(ECharacterSkills::DEBUFFSPELLS));
	StringToSet.Append("/100");
	DebuffSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Restoration spells: ");
	StringToSet.AppendInt(Ally->GetSkill(ECharacterSkills::RESTORATIONSPELLS));
	StringToSet.Append("/100");
	RestorationSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Buff spells: ");
	StringToSet.AppendInt(Ally->GetSkill(ECharacterSkills::BUFFSPELLS));
	StringToSet.Append("/100");
	BuffSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Defend: ");
	StringToSet.AppendInt(Ally->GetSkill(ECharacterSkills::DEFEND));
	StringToSet.Append("/100");
	DefendSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Persuasion: ");
	StringToSet.AppendInt(Ally->GetSkill(ECharacterSkills::PERSUASION));
	StringToSet.Append("/100");
	PersuasionSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Melee attack: ");
	StringToSet.AppendInt(Ally->GetMeleeAttackValue());
	MeleeAttackTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Range attack: ");
	StringToSet.AppendInt(Ally->GetRangeAttackValue());
	RangeAttackTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Range ammo: ");
	StringToSet.AppendInt(Ally->GetRangeAmmo());
	RangeAmmoTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Armor: ");
	StringToSet.AppendInt(Ally->GetArmorValue());
	ArmorTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Evasion chance: ");
	StringToSet.AppendInt(Ally->GetEvasionChance() + Ally->GetStat(ECharacterStats::AGILITY) + Ally->GetStat(ECharacterStats::LUCK));
	EvasionChanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Targeting chance: ");
	StringToSet.AppendInt(Ally ->TargetingChance);
	TargetingChanceTextBlock->SetText(FText::FromString(StringToSet));\
	StringToSet.Empty();
	StringToSet.Append("Fire resistance: ");
	for(FElementAndItsPercentageStruct ElementPercent : Ally->GetElementalResistances())
		if (ElementPercent.Element == ESpellElements::FIRE) {
			StringToSet.AppendInt(ElementPercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	FireResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Water resistance: ");
	for (FElementAndItsPercentageStruct ElementPercent : Ally->GetElementalResistances())
		if (ElementPercent.Element == ESpellElements::WATER) {
			StringToSet.AppendInt(ElementPercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	WaterResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Wind resistance: ");
	for (FElementAndItsPercentageStruct ElementPercent : Ally->GetElementalResistances())
		if (ElementPercent.Element == ESpellElements::WIND) {
			StringToSet.AppendInt(ElementPercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	WindResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Earth resistance: ");
	for (FElementAndItsPercentageStruct ElementPercent : Ally->GetElementalResistances())
		if (ElementPercent.Element == ESpellElements::EARTH) {
			StringToSet.AppendInt(ElementPercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	EarthResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Lightning resistance: ");
	for (FElementAndItsPercentageStruct ElementPercent : Ally->GetElementalResistances())
		if (ElementPercent.Element == ESpellElements::LIGHTNING) {
			StringToSet.AppendInt(ElementPercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	LightningResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Holy resistance: ");
	for (FElementAndItsPercentageStruct ElementPercent : Ally->GetElementalResistances())
		if (ElementPercent.Element == ESpellElements::HOLY) {
			StringToSet.AppendInt(ElementPercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	HolyResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Dark resistance: ");
	for (FElementAndItsPercentageStruct ElementPercent : Ally->GetElementalResistances())
		if (ElementPercent.Element == ESpellElements::DARK) {
			StringToSet.AppendInt(ElementPercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	DarkResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Blood resistance: ");
	for (FElementAndItsPercentageStruct ElementPercent : Ally->GetElementalResistances())
		if (ElementPercent.Element == ESpellElements::BLOOD) {
			StringToSet.AppendInt(ElementPercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	BloodResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Multielemental resistance: ");
	for (FElementAndItsPercentageStruct ElementPercent : Ally->GetElementalResistances())
		if (ElementPercent.Element == ESpellElements::MULTIELEMENTAL) {
			StringToSet.AppendInt(ElementPercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	MultielementalResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Slashing resistance: ");
	for (FPhysicalTypeAndItsPercentageStruct PhysicalTypePercent : Ally->GetPhysicalResistances())
		if (PhysicalTypePercent.PhysicalType == EPhysicalType::SLASHING) {
			StringToSet.AppendInt(PhysicalTypePercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	SlashingResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Crushing resistance: ");
	for (FPhysicalTypeAndItsPercentageStruct PhysicalTypePercent : Ally->GetPhysicalResistances())
		if (PhysicalTypePercent.PhysicalType == EPhysicalType::CRUSHING) {
			StringToSet.AppendInt(PhysicalTypePercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	CrushingResistanceTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Piercing resistance: ");
	for (FPhysicalTypeAndItsPercentageStruct PhysicalTypePercent : Ally->GetPhysicalResistances())
		if (PhysicalTypePercent.PhysicalType == EPhysicalType::PIERCING) {
			StringToSet.AppendInt(PhysicalTypePercent.Percent);
			break;
		}
	StringToSet.AppendChar('%');
	PiercingResistanceTextBlock->SetText(FText::FromString(StringToSet));
	CharacterPortraitImage->Brush.SetResourceObject(const_cast<UTexture*>(Ally->GetCharacterPortrait()));
	CharacterPortraitImage->Brush.SetImageSize(FVector2D(120, 100));
}

void UDetailedCharacterInfoMenu::SetCharacterInfoForPlayer(const APlayerCharacter* const Player)
{
	if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		CharacterNameTextBlock->SetText(FText::FromString(Player->CharacterName));
		FString StringToSet{};
		StringToSet.Append("Level: ");
		StringToSet.AppendInt(Player->Level);
		CharacterLevelTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("HP: ");
		StringToSet.AppendInt(Player->CurrentHP);
		StringToSet.Append("/");
		StringToSet.AppendInt(Player->MaxHP);
		CharacterHPTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Mana: ");
		StringToSet.AppendInt(Player->CurrentMana);
		StringToSet.Append("/");
		StringToSet.AppendInt(Player->MaxMana);
		CharacterManaTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Strength: ");
		StringToSet.AppendInt(Player->GetStat(ECharacterStats::STRENGTH));
		StrengthStatTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Perception: ");
		StringToSet.AppendInt(Player->GetStat(ECharacterStats::PERCEPTION));
		PerceptionStatTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Endurance: ");
		StringToSet.AppendInt(Player->GetStat(ECharacterStats::ENDURANCE));
		EnduranceStatTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Charisma: ");
		StringToSet.AppendInt(Player->GetStat(ECharacterStats::CHARISMA));
		CharismaStatTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Intelligence: ");
		StringToSet.AppendInt(Player->GetStat(ECharacterStats::INTELLIGENCE));
		IntelligenceStatTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Will: ");
		StringToSet.AppendInt(Player->GetStat(ECharacterStats::WILL));
		WillStatTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Agility: ");
		StringToSet.AppendInt(Player->GetStat(ECharacterStats::AGILITY));
		AgilityStatTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Luck: ");
		StringToSet.AppendInt(Player->GetStat(ECharacterStats::LUCK));
		LuckStatTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Melee: ");
		StringToSet.AppendInt(Player->GetSkill(ECharacterSkills::MELEE));
		StringToSet.Append("/100");
		MeleeSkillTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Range: ");
		StringToSet.AppendInt(Player->GetSkill(ECharacterSkills::RANGE));
		StringToSet.Append("/100");
		RangeSkillTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Assault spells: ");
		StringToSet.AppendInt(Player->GetSkill(ECharacterSkills::ASSAULTSPELLS));
		StringToSet.Append("/100");
		AssaultSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Debuff spells: ");
		StringToSet.AppendInt(Player->GetSkill(ECharacterSkills::DEBUFFSPELLS));
		StringToSet.Append("/100");
		DebuffSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Restoration spells: ");
		StringToSet.AppendInt(Player->GetSkill(ECharacterSkills::RESTORATIONSPELLS));
		StringToSet.Append("/100");
		RestorationSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Buff spells: ");
		StringToSet.AppendInt(Player->GetSkill(ECharacterSkills::BUFFSPELLS));
		StringToSet.Append("/100");
		BuffSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Defend: ");
		StringToSet.AppendInt(Player->GetSkill(ECharacterSkills::DEFEND));
		StringToSet.Append("/100");
		DefendSkillTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Persuasion: ");
		StringToSet.AppendInt(Player->GetSkill(ECharacterSkills::PERSUASION));
		StringToSet.Append("/100");
		PersuasionSkillTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Melee attack: ");
		if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedMelee))
			StringToSet.AppendInt(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedMelee->GetAttackValue());
		else
			StringToSet.AppendInt(5);
		MeleeAttackTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Range attack: ");
		if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedRange))
			StringToSet.AppendInt(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedRange->GetAttackValue());
		else
			StringToSet.AppendInt(0);
		RangeAttackTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Range ammo: ");
		if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedRange))
			StringToSet.AppendInt(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedRange->GetAmmo());
		else
			StringToSet.AppendInt(0);
		RangeAmmoTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Armor: ");
		uint16 ArmorSum = 0;
		if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead))
			ArmorSum += UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetArmorValue();
		if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse))
			ArmorSum += UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetArmorValue();
		if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand))
			ArmorSum += UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetArmorValue();
		if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor))
			ArmorSum += UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetArmorValue();
		StringToSet.AppendInt(ArmorSum);
		ArmorTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Evasion chance: ");
		float EvasionChance = Player->EvasionChance + Player->GetStat(ECharacterStats::LUCK) + Player->GetStat(ECharacterStats::AGILITY);
		StringToSet.AppendInt(EvasionChance);
		EvasionChanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Targeting chance: ");
		StringToSet.AppendInt(Player->TargetingChance);
		TargetingChanceTextBlock->SetText(FText::FromString(StringToSet));
		//Get elemental resistances from inventory items
		TArray<FElementAndItsPercentageStruct> PlayerElementalResistances{};
		SkillsSpellsAndEffectsActions::InitializeElementalResistances(PlayerElementalResistances);
		for (int i = 0; i < PlayerElementalResistances.Num(); i++) {
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetElementsAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetElementsAndTheirPercentagesStructs()[g].Element == PlayerElementalResistances[i].Element) {
						PlayerElementalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetElementsAndTheirPercentagesStructs()[g].Percent + PlayerElementalResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetElementsAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetElementsAndTheirPercentagesStructs()[g].Element == PlayerElementalResistances[i].Element) {
						PlayerElementalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetElementsAndTheirPercentagesStructs()[g].Percent + PlayerElementalResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetElementsAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetElementsAndTheirPercentagesStructs()[g].Element == PlayerElementalResistances[i].Element) {
						PlayerElementalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetElementsAndTheirPercentagesStructs()[g].Percent + PlayerElementalResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetElementsAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetElementsAndTheirPercentagesStructs()[g].Element == PlayerElementalResistances[i].Element) {
						PlayerElementalResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetElementsAndTheirPercentagesStructs()[g].Percent + PlayerElementalResistances[i].Percent;
					}
		}
		StringToSet.Empty();
		StringToSet.Append("Fire resistance: ");
		for (FElementAndItsPercentageStruct ElementPercent : PlayerElementalResistances)
			if (ElementPercent.Element == ESpellElements::FIRE) {
				StringToSet.AppendInt(ElementPercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		FireResistanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Water resistance: ");
		for (FElementAndItsPercentageStruct ElementPercent : PlayerElementalResistances)
			if (ElementPercent.Element == ESpellElements::WATER) {
				StringToSet.AppendInt(ElementPercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		WaterResistanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Wind resistance: ");
		for (FElementAndItsPercentageStruct ElementPercent : PlayerElementalResistances)
			if (ElementPercent.Element == ESpellElements::WIND) {
				StringToSet.AppendInt(ElementPercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		WindResistanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Earth resistance: ");
		for (FElementAndItsPercentageStruct ElementPercent : PlayerElementalResistances)
			if (ElementPercent.Element == ESpellElements::EARTH) {
				StringToSet.AppendInt(ElementPercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		EarthResistanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Lightning resistance: ");
		for (FElementAndItsPercentageStruct ElementPercent : PlayerElementalResistances)
			if (ElementPercent.Element == ESpellElements::LIGHTNING) {
				StringToSet.AppendInt(ElementPercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		LightningResistanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Holy resistance: ");
		for (FElementAndItsPercentageStruct ElementPercent : PlayerElementalResistances)
			if (ElementPercent.Element == ESpellElements::HOLY) {
				StringToSet.AppendInt(ElementPercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		HolyResistanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Dark resistance: ");
		for (FElementAndItsPercentageStruct ElementPercent : PlayerElementalResistances)
			if (ElementPercent.Element == ESpellElements::DARK) {
				StringToSet.AppendInt(ElementPercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		DarkResistanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Blood resistance: ");
		for (FElementAndItsPercentageStruct ElementPercent : PlayerElementalResistances)
			if (ElementPercent.Element == ESpellElements::BLOOD) {
				StringToSet.AppendInt(ElementPercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		BloodResistanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Multielemental resistance: ");
		for (FElementAndItsPercentageStruct ElementPercent : PlayerElementalResistances)
			if (ElementPercent.Element == ESpellElements::MULTIELEMENTAL) {
				StringToSet.AppendInt(ElementPercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		MultielementalResistanceTextBlock->SetText(FText::FromString(StringToSet));
		//Get physical resistances.
		TArray<FPhysicalTypeAndItsPercentageStruct> PlayerPhysicalTypesResistances{};
		SkillsSpellsAndEffectsActions::InitializePhysicalResistances(PlayerPhysicalTypesResistances);
		for (int i = 0; i < PlayerPhysicalTypesResistances.Num(); i++) {
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PlayerPhysicalTypesResistances[i].PhysicalType) {
						PlayerPhysicalTypesResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHead->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PlayerPhysicalTypesResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PlayerPhysicalTypesResistances[i].PhysicalType) {
						PlayerPhysicalTypesResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedTorse->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PlayerPhysicalTypesResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PlayerPhysicalTypesResistances[i].PhysicalType) {
						PlayerPhysicalTypesResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedHand->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PlayerPhysicalTypesResistances[i].Percent;
					}
			if (IsValid(UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor))
				for (int g = 0; g < UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetPhysicalTypesAndTheirPercentagesStructs().Num(); g++)
					if (UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetPhysicalTypesAndTheirPercentagesStructs()[g].PhysicalType == PlayerPhysicalTypesResistances[i].PhysicalType) {
						PlayerPhysicalTypesResistances[i].Percent = UIManagerWorldSubsystem->InventoryMenuWidget->EquipedLowerArmor->GetPhysicalTypesAndTheirPercentagesStructs()[g].Percent + PlayerPhysicalTypesResistances[i].Percent;
					}
		}
		StringToSet.Empty();
		StringToSet.Append("Slashing resistance: ");
		for (FPhysicalTypeAndItsPercentageStruct PhysicalTypePercent : PlayerPhysicalTypesResistances)
			if (PhysicalTypePercent.PhysicalType == EPhysicalType::SLASHING) {
				StringToSet.AppendInt(PhysicalTypePercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		SlashingResistanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Crushing resistance: ");
		for (FPhysicalTypeAndItsPercentageStruct PhysicalTypePercent : PlayerPhysicalTypesResistances)
			if (PhysicalTypePercent.PhysicalType == EPhysicalType::CRUSHING) {
				StringToSet.AppendInt(PhysicalTypePercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		CrushingResistanceTextBlock->SetText(FText::FromString(StringToSet));
		StringToSet.Empty();
		StringToSet.Append("Piercing resistance: ");
		for (FPhysicalTypeAndItsPercentageStruct PhysicalTypePercent : PlayerPhysicalTypesResistances)
			if (PhysicalTypePercent.PhysicalType == EPhysicalType::PIERCING) {
				StringToSet.AppendInt(PhysicalTypePercent.Percent);
				break;
			}
		StringToSet.AppendChar('%');
		PiercingResistanceTextBlock->SetText(FText::FromString(StringToSet));
		CharacterPortraitImage->Brush.SetResourceObject(const_cast<UTexture*>(Player->CharacterPortrait));
		CharacterPortraitImage->Brush.SetImageSize(FVector2D(120, 100));
	}
}

void UDetailedCharacterInfoMenu::BackButtonOnClicked()
{
	this->RemoveFromParent();
	this->ConditionalBeginDestroy();
	if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->PartyMenuWidget->AddToViewport();
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		if (auto* GeneralCharacterInfoWidget = Cast<UPartyMenuGeneralCharacterInfo>
			(UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetChildAt(0)); IsValid(GeneralCharacterInfoWidget)) {
			UIManagerWorldSubsystem->PickedButton = GeneralCharacterInfoWidget->GetCharacterNameButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex = 0;
		}
	}
	if (MoreInfoShowed) {
		SkillsBorder->SetVisibility(ESlateVisibility::Visible);
		StatsBorder->SetVisibility(ESlateVisibility::Visible);
		PhysicalResistancesBorder->SetVisibility(ESlateVisibility::Hidden);
		ElementalResistancesBorder->SetVisibility(ESlateVisibility::Hidden);
		MoreInfoShowed = false;
	}
}

void UDetailedCharacterInfoMenu::BackButtonOnHovered()
{
	ButtonOnHoveredActions(BackButton, 2);
}

void UDetailedCharacterInfoMenu::AbilitiesButtonOnClicked()
{
	if(auto* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			this->RemoveFromParent();
			UIManagerWorldSubsystem->SkillBattleMenuWidget->ResetSkillsScrollBox();
			if (Ally != nullptr) {
				for (TSubclassOf<ASpell> SpellClass : Ally->GetAvailableSpells())
					UIManagerWorldSubsystem->SkillBattleMenuWidget->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
			}
			else {
				for (TSubclassOf<ASpell> SpellClass : PlayerCharacter->GetAvailableSkills())
					UIManagerWorldSubsystem->SkillBattleMenuWidget->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
			}
			UIManagerWorldSubsystem->SkillBattleMenuWidget->AddToViewport();
			UIManagerWorldSubsystem->SkillBattleMenuWidget->GetUseButtonWithNeighbors()->SetVisibility(ESlateVisibility::Hidden);
			UIManagerWorldSubsystem->SkillBattleMenuWidget->IsOpenedFromDetailedCharacterInfo = true;
			AbilitiesButton->SetBackgroundColor(FLinearColor(0.7f, 0.7f, 0.7f, 1.f));
		}
}

void UDetailedCharacterInfoMenu::AbilitiesButtonOnHovered()
{
	ButtonOnHoveredActions(AbilitiesButton, 1);
}

void UDetailedCharacterInfoMenu::ToggleInfoButtonOnClicked()
{
	if (MoreInfoShowed) {
		SkillsBorder->SetVisibility(ESlateVisibility::Visible);
		StatsBorder->SetVisibility(ESlateVisibility::Visible);
		PhysicalResistancesBorder->SetVisibility(ESlateVisibility::Hidden);
		ElementalResistancesBorder->SetVisibility(ESlateVisibility::Hidden);
		MoreInfoShowed = false;
	}
	else {
		SkillsBorder->SetVisibility(ESlateVisibility::Hidden);
		StatsBorder->SetVisibility(ESlateVisibility::Hidden);
		PhysicalResistancesBorder->SetVisibility(ESlateVisibility::Visible);
		ElementalResistancesBorder->SetVisibility(ESlateVisibility::Visible);
		MoreInfoShowed = true;
	}
}

void UDetailedCharacterInfoMenu::ToggleInfoButtonOnHovered()
{
	ButtonOnHoveredActions(ToggleInfoButton, 0);
}

void UDetailedCharacterInfoMenu::ButtonOnHoveredActions(const UButton* const HoveredButton, const uint8 Index)
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.7f, 0.7f, 0.7f, 1.f));
		UIManagerWorldSubsystem->PickedButton = const_cast<UButton*>(HoveredButton);
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButtonIndex = Index;
	}
}

UButton* UDetailedCharacterInfoMenu::GetBackButton() const
{
	return BackButton;
}

UButton* UDetailedCharacterInfoMenu::GetAbilitiesButton() const
{
	return AbilitiesButton;
}

UButton* UDetailedCharacterInfoMenu::GetToggleInfoButton() const
{
	return ToggleInfoButton;
}

UBorder* UDetailedCharacterInfoMenu::GetSkillsBorder() const
{
	return SkillsBorder;
}

UBorder* UDetailedCharacterInfoMenu::GetStatsBorder() const
{
	return StatsBorder;
}

UBorder* UDetailedCharacterInfoMenu::GetPhysicalResistancesBorder() const
{
	return PhysicalResistancesBorder;
}

UBorder* UDetailedCharacterInfoMenu::GetElementalResistancesBorder() const
{
	return ElementalResistancesBorder;
}

UVerticalBox* UDetailedCharacterInfoMenu::GetButtonsVerticalBox() const
{
	return ButtonsVerticalBox;
}
