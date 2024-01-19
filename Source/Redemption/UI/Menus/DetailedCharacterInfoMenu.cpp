// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailedCharacterInfoMenu.h"

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
	if (!bSuccess) return false;
	return bSuccess;
}

void UDetailedCharacterInfoMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDetailedCharacterInfoMenu::SetCharacterInfo(const ACombatAllyNPC* const AllyToSet)
{
	CharacterNameTextBlock->SetText(FText::FromName(AllyToSet->GetCharacterName()));
	FString StringToSet{};
	StringToSet.Append("Level: ");
	StringToSet.AppendInt(AllyToSet->Level);
	CharacterLevelTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("HP: ");
	StringToSet.AppendInt(AllyToSet->CurrentHP);
	StringToSet.Append("/");
	StringToSet.AppendInt(AllyToSet->MaxHP);
	CharacterHPTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Mana: ");
	StringToSet.AppendInt(AllyToSet->CurrentMana);
	StringToSet.Append("/");
	StringToSet.AppendInt(AllyToSet->MaxMana);
	CharacterManaTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Strength: ");
	StringToSet.AppendInt(AllyToSet->GetStat(ECharacterStats::STRENGTH));
	StrengthStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Perception: ");
	StringToSet.AppendInt(AllyToSet->GetStat(ECharacterStats::PERCEPTION));
	PerceptionStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Endurance: ");
	StringToSet.AppendInt(AllyToSet->GetStat(ECharacterStats::ENDURANCE));
	EnduranceStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Charisma: ");
	StringToSet.AppendInt(AllyToSet->GetStat(ECharacterStats::CHARISMA));
	CharismaStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Intelligence: ");
	StringToSet.AppendInt(AllyToSet->GetStat(ECharacterStats::INTELLIGENCE));
	IntelligenceStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Will: ");
	StringToSet.AppendInt(AllyToSet->GetStat(ECharacterStats::WILL));
	WillStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Agility: ");
	StringToSet.AppendInt(AllyToSet->GetStat(ECharacterStats::AGILITY));
	AgilityStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Luck: ");
	StringToSet.AppendInt(AllyToSet->GetStat(ECharacterStats::LUCK));
	LuckStatTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Melee: ");
	StringToSet.AppendInt(AllyToSet->GetSkill(ECharacterSkills::MELEE));
	StringToSet.Append("/100");
	MeleeSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Range: ");
	StringToSet.AppendInt(AllyToSet->GetSkill(ECharacterSkills::RANGE));
	StringToSet.Append("/100");
	RangeSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Assault spells: ");
	StringToSet.AppendInt(AllyToSet->GetSkill(ECharacterSkills::ASSAULTSPELLS));
	StringToSet.Append("/100");
	AssaultSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Debuff spells: ");
	StringToSet.AppendInt(AllyToSet->GetSkill(ECharacterSkills::DEBUFFSPELLS));
	StringToSet.Append("/100");
	DebuffSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Restoration spells: ");
	StringToSet.AppendInt(AllyToSet->GetSkill(ECharacterSkills::RESTORATIONSPELLS));
	StringToSet.Append("/100");
	RestorationSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Buff spells: ");
	StringToSet.AppendInt(AllyToSet->GetSkill(ECharacterSkills::BUFFSPELLS));
	StringToSet.Append("/100");
	BuffSpellsSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Defend: ");
	StringToSet.AppendInt(AllyToSet->GetSkill(ECharacterSkills::DEFEND));
	StringToSet.Append("/100");
	DefendSkillTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Persuasion: ");
	StringToSet.AppendInt(AllyToSet->GetSkill(ECharacterSkills::PERSUASION));
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
	TargetingChanceTextBlock->SetText(FText::FromString(StringToSet));
	CharacterPortraitImage->Brush.SetResourceObject(const_cast<UTexture*>(AllyToSet->GetCharacterPortrait()));
	CharacterPortraitImage->Brush.SetImageSize(FVector2D(120, 100));
}

void UDetailedCharacterInfoMenu::SetCharacterInfo(const APlayerCharacter* const Player)
{
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
	if(IsValid(Player->GetInventoryMenuWidget()->EquipedMelee))
		StringToSet.AppendInt(Player->GetInventoryMenuWidget()->EquipedMelee->GetAttackValue());
	else
		StringToSet.AppendInt(5);
	MeleeAttackTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Range attack: ");
	if (IsValid(Player->GetInventoryMenuWidget()->EquipedRange))
		StringToSet.AppendInt(Player->GetInventoryMenuWidget()->EquipedRange->GetAttackValue());
	else
		StringToSet.AppendInt(0);
	RangeAttackTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Range ammo: ");
	if (IsValid(Player->GetInventoryMenuWidget()->EquipedRange))
		StringToSet.AppendInt(Player->GetInventoryMenuWidget()->EquipedRange->GetAmmo());
	else
		StringToSet.AppendInt(0);
	RangeAmmoTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Armor: ");
	uint16 ArmorSum = 0;
	if (IsValid(Player->GetInventoryMenuWidget()->EquipedHead))
		ArmorSum += Player->GetInventoryMenuWidget()->EquipedHead->GetArmorValue();
	if (IsValid(Player->GetInventoryMenuWidget()->EquipedTorse))
		ArmorSum += Player->GetInventoryMenuWidget()->EquipedTorse->GetArmorValue();
	if (IsValid(Player->GetInventoryMenuWidget()->EquipedHand))
		ArmorSum += Player->GetInventoryMenuWidget()->EquipedHand->GetArmorValue();
	if (IsValid(Player->GetInventoryMenuWidget()->EquipedLowerArmor))
		ArmorSum += Player->GetInventoryMenuWidget()->EquipedLowerArmor->GetArmorValue();
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
	CharacterPortraitImage->Brush.SetResourceObject(const_cast<UTexture*>(Player->CharacterPortrait));
	CharacterPortraitImage->Brush.SetImageSize(FVector2D(120, 100));
}

void UDetailedCharacterInfoMenu::BackButtonOnClicked()
{
	this->RemoveFromParent();
	if (auto* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		PlayerCharacter->GetPartyMenuWidget()->AddToViewport();
		if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
			UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetPartyMenuWidget()->GetBackButton();
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		}
	}
}

void UDetailedCharacterInfoMenu::BackButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		UIManagerWorldSubsystem->PickedButton = BackButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButtonIndex = 1;
	}
}

void UDetailedCharacterInfoMenu::AbilitiesButtonOnClicked()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())) {
		this->RemoveFromParent();
		PlayerCharacter->GetSkillBattleMenuWidget()->AddToViewport();
		PlayerCharacter->GetSkillBattleMenuWidget()->ResetSkillsScrollBox();
		if (Ally != nullptr) {
			for (TSubclassOf<ASpell> SpellClass : Ally->GetAvailableSpells())
				PlayerCharacter->GetSkillBattleMenuWidget()->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
		}
		else {
			for (TSubclassOf<ASpell> SpellClass : PlayerCharacter->GetAvailableSkills())
				PlayerCharacter->GetSkillBattleMenuWidget()->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
		}
	}
}

void UDetailedCharacterInfoMenu::AbilitiesButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		UIManagerWorldSubsystem->PickedButton = AbilitiesButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

UButton* UDetailedCharacterInfoMenu::GetBackButton() const
{
	return BackButton;
}