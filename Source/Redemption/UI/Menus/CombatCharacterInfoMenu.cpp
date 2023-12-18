// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacterInfoMenu.h"
#include "..\Characters\Combat\CombatNPC.h"
#include "..\Characters\Player\PlayerCharacter.h"

bool UCombatCharacterInfoMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(BackButtonWithNeighbors)) {
		BackButtonWithNeighbors->OnClicked.AddDynamic(this, &UCombatCharacterInfoMenu::BackButtonOnClicked);
		BackButtonWithNeighbors->OnHovered.AddDynamic(this, &UCombatCharacterInfoMenu::BackButtonOnHovered);
	}
	if (IsValid(EffectsResistancesToggleButtonWithNeighbors)) {
		EffectsResistancesToggleButtonWithNeighbors->OnClicked.AddDynamic(this, &UCombatCharacterInfoMenu::EffectsResistancesToggleButtonOnClicked);
		EffectsResistancesToggleButtonWithNeighbors->OnHovered.AddDynamic(this, &UCombatCharacterInfoMenu::EffectsResistancesToggleButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UCombatCharacterInfoMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCombatCharacterInfoMenu::BackButtonOnClicked()
{
	if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget(); IsValid(BattleMenu)) {
			this->RemoveFromParent();
			EffectInfoBorder->SetVisibility(ESlateVisibility::Hidden);
			GeneralInformationBorder->SetVisibility(ESlateVisibility::Visible);
			ResistancesBorder->SetVisibility(ESlateVisibility::Hidden);
			EffectsResistancesToggleTextBlock->SetText(FText::FromString("View resistances"));
			if (IsValid(PlayerCharacter->GetUIManagerWorldSubsystem())) {
				if (IsValid(PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton))
					PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
				PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton = BattleMenu->GetAttackTalkInfoActionButton();
				PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButtonIndex = 0;
				PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			BattleMenu->AddToViewport();
			if (IsValid(PlayerCharacter->GetBattleManager()) && IsValid(PlayerCharacter->GetBattleManager()->SelectedCombatNPC))
				PlayerCharacter->GetBattleManager()->SelectedCombatNPC->GetCrosshairWidgetComponent()->SetVisibility(false);
			BattleMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
			BattleMenu->GetLeftRightMenuBorder()->SetVisibility(ESlateVisibility::Visible);
			BattleMenu->GetAttackMenuBorder()->SetVisibility(ESlateVisibility::Visible);
		}
}

void UCombatCharacterInfoMenu::EffectsResistancesToggleButtonOnClicked()
{
	if (GeneralInformationBorder->GetVisibility() == ESlateVisibility::Visible) {
		GeneralInformationBorder->SetVisibility(ESlateVisibility::Hidden);
		ResistancesBorder->SetVisibility(ESlateVisibility::Visible);
		EffectsResistancesToggleTextBlock->SetText(FText::FromString("View general information and effects"));
	}
	else if (ResistancesBorder->GetVisibility() == ESlateVisibility::Visible) {
		GeneralInformationBorder->SetVisibility(ESlateVisibility::Visible);
		ResistancesBorder->SetVisibility(ESlateVisibility::Hidden);
		EffectsResistancesToggleTextBlock->SetText(FText::FromString("View resistances"));
	}
}

void UCombatCharacterInfoMenu::BackButtonOnHovered()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (IsValid(PlayerCharacter->GetUIManagerWorldSubsystem())) {
			if (IsValid(PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton))
				PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
			PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton = BackButtonWithNeighbors;
			PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButtonIndex = 0;
			PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		}
	CanUseKeyboardButtonSelection = false;
}

void UCombatCharacterInfoMenu::EffectsResistancesToggleButtonOnHovered()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (IsValid(PlayerCharacter->GetUIManagerWorldSubsystem())) {
			if (IsValid(PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton))
				PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
			PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton = EffectsResistancesToggleButtonWithNeighbors;
			PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButtonIndex = 0;
			PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		}
	CanUseKeyboardButtonSelection = false;
}

void UCombatCharacterInfoMenu::ResetActiveEffectsScrollBox()
{
	ActiveEffectsScrollBox->ClearChildren();
	ActiveEffectEntryWidget = nullptr;
}

void UCombatCharacterInfoMenu::SetCharacterInfo(const ACombatNPC* const NPCToViewInfoOf)
{
	FString CharacterNameString = "Name: " + NPCToViewInfoOf->GetCharacterName().ToString();
	CharacterNameTextBlock->SetText(FText::FromString(CharacterNameString));
	FString CharacterHPString = "HP : " + FString::FromInt(NPCToViewInfoOf->CurrentHP) + '/' + FString::FromInt(NPCToViewInfoOf->MaxHP);
	CharacterHPTextBlock->SetText(FText::FromString(CharacterHPString));
	for (AEffect* Effect : NPCToViewInfoOf->Effects)
		AddActiveEffectEntryToActiveEffectsScrollBox(Effect);
}

void UCombatCharacterInfoMenu::SetEffectInfo(const AEffect* const EffectToViewInfoOf)
{
	if (EffectInfoBorder->GetVisibility() != ESlateVisibility::Visible)
		EffectInfoBorder->SetVisibility(ESlateVisibility::Visible);
	FString StringToSet{};
	StringToSet = "Name: " + EffectToViewInfoOf->GetEffectName().ToString();
	EffectNameTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet = "Area: " + UEnum::GetDisplayValueAsText(EffectToViewInfoOf->GetEffectArea()).ToString();
	EffectAreaTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet = "Type: " + UEnum::GetDisplayValueAsText(EffectToViewInfoOf->GetEffectType()).ToString();
	EffectTypeTextBlock->SetText(FText::FromString(StringToSet));
	switch (EffectToViewInfoOf->GetEffectType()) {
		case EEffectType::BUFF:
			StringToSet = "Effect Value: multiply by ";
			break;
		case EEffectType::DEBUFF:
			StringToSet = "Effect Value: divide by ";
			break;
		case EEffectType::PLAINBUFF:
			StringToSet = "Effect Value: add ";
			break;
		case EEffectType::PLAINDEBUFF:
			StringToSet = "Effect Value: substract ";
			break;
		default: 
			StringToSet = "Effect Value: ";
			break;
	}
	StringToSet += FString::FromInt(EffectToViewInfoOf->GetEffectStat());
	EffectValueTextBlock->SetText(FText::FromString(StringToSet));
	int8 EffectDuration = EffectToViewInfoOf->GetDuration() - EffectToViewInfoOf->CurrentDuration;
	StringToSet = "Duration: " + FString::FromInt(EffectDuration + 1);
	if (EffectDuration > 1)
		StringToSet += " turns left";
	else
		StringToSet += " turn left";
	EffectDurationTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet = "Description: " + EffectToViewInfoOf->GetEffectDescription().ToString();
	EffectDescriptionTextBlock->SetText(FText::FromString(StringToSet));
}

void UCombatCharacterInfoMenu::SetCharacterResistances(const ACombatNPC* const NPCToViewInfoOf)
{
	FString TextBlockString{};
	for (FElementAndItsPercentageStruct ElementPercent : NPCToViewInfoOf->GetElementalResistances())
		switch (ElementPercent.Element) {
			case ESpellElements::FIRE:
				TextBlockString = "Fire resistance :";
				TextBlockString.AppendInt(ElementPercent.Percent);
				FireResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
			case ESpellElements::WATER:
				TextBlockString = "Water resistance :";
				TextBlockString.AppendInt(ElementPercent.Percent);
				WaterResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
			case ESpellElements::WIND:
				TextBlockString = "Wind resistance :";
				TextBlockString.AppendInt(ElementPercent.Percent);
				WindResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
			case ESpellElements::EARTH:
				TextBlockString = "Earth resistance :";
				TextBlockString.AppendInt(ElementPercent.Percent);
				EarthResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
			case ESpellElements::LIGHTNING:
				TextBlockString = "Lightning resistance :";
				TextBlockString.AppendInt(ElementPercent.Percent);
				LightningResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
			case ESpellElements::HOLY:
				TextBlockString = "Holy resistance :";
				TextBlockString.AppendInt(ElementPercent.Percent);
				HolyResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
			case ESpellElements::DARK:
				TextBlockString = "Dark resistance :";
				TextBlockString.AppendInt(ElementPercent.Percent);
				DarkResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
			case ESpellElements::BLOOD:
				TextBlockString = "Blood resistance :";
				TextBlockString.AppendInt(ElementPercent.Percent);
				BloodResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
			case ESpellElements::MULTIELEMENTAL:
				TextBlockString = "Multielemental resistance :";
				TextBlockString.AppendInt(ElementPercent.Percent);
				MultielementalResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
		}
	for (FPhysicalTypeAndItsPercentageStruct PhysicalTypePercent : NPCToViewInfoOf->GetPhysicalResistances())
		switch (PhysicalTypePercent.PhysicalType) {
			case EPhysicalType::SLASHING:
				TextBlockString = "Slashing resistance :";
				TextBlockString.AppendInt(PhysicalTypePercent.Percent);
				SlashingResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
			case EPhysicalType::CRUSHING:
				TextBlockString = "Crushing resistance :";
				TextBlockString.AppendInt(PhysicalTypePercent.Percent);
				CrushingResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
			case EPhysicalType::PIERCING:
				TextBlockString = "Piercing resistance :";
				TextBlockString.AppendInt(PhysicalTypePercent.Percent);
				PiercingResistanceTextBlock->SetText(FText::FromString(TextBlockString));
				break;
		}
}

void UCombatCharacterInfoMenu::AddActiveEffectEntryToActiveEffectsScrollBox(const class AEffect* const EffectToAdd)
{
	if (IsValid(ActiveEffectEntryClass))
		ActiveEffectEntryWidget = CreateWidget<UActiveEffectEntryWidget>(GetWorld(), ActiveEffectEntryClass);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());IsValid(PlayerCharacter) && IsValid(ActiveEffectEntryWidget)) {
		ActiveEffectEntryWidget->SetEffectNameText(EffectToAdd->GetEffectName());
		ActiveEffectEntryWidget->SetEffectTypeImage(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetEffectTypeImageTexture(EffectToAdd->GetEffectType()));
		ActiveEffectEntryWidget->SetEffectAreaImage(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetEffectAreaImageTexture(EffectToAdd->GetEffectArea()));;
		ActiveEffectEntryWidget->EntryEffect = const_cast<AEffect*>(EffectToAdd);
		ActiveEffectsScrollBox->AddChild(ActiveEffectEntryWidget);
	}
}

UTextBlock* UCombatCharacterInfoMenu::GetCharacterNameTextBlock() const
{
	return CharacterNameTextBlock;
}

UTextBlock* UCombatCharacterInfoMenu::GetCharacterHPTextBlock() const
{
	return CharacterHPTextBlock;
}

UTextBlock* UCombatCharacterInfoMenu::GetEffectNameTextBlock() const
{
	return EffectNameTextBlock;
}

UTextBlock* UCombatCharacterInfoMenu::GetEffectAreaTextBlock() const
{
	return EffectAreaTextBlock;
}

UTextBlock* UCombatCharacterInfoMenu::GetEffectTypeTextBlock() const
{
	return EffectTypeTextBlock;
}

UTextBlock* UCombatCharacterInfoMenu::GetEffectValueTextBlock() const
{
	return EffectValueTextBlock;
}

UTextBlock* UCombatCharacterInfoMenu::GetEffectDurationTextBlock() const
{
	return EffectDurationTextBlock;
}

UTextBlock* UCombatCharacterInfoMenu::GetEffectDescriptionTextBlock() const
{
	return EffectDescriptionTextBlock;
}

UScrollBox* UCombatCharacterInfoMenu::GetActiveEffectsScrollBox() const
{
	return ActiveEffectsScrollBox;
}

UButtonWithNeighbors* UCombatCharacterInfoMenu::GetBackButtonWithNeighbors() const
{
	return BackButtonWithNeighbors;
}

UButtonWithNeighbors* UCombatCharacterInfoMenu::GetEffectsResistancesToggleButtonWithNeighbors() const
{
	return EffectsResistancesToggleButtonWithNeighbors;
}

UBorder* UCombatCharacterInfoMenu::GetEffectInfoBorder() const
{
	return EffectInfoBorder;
}
