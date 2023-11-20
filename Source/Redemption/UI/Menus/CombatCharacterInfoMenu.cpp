// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacterInfoMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"

bool UCombatCharacterInfoMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(BackButton)) {
		BackButton->OnClicked.AddDynamic(this, &UCombatCharacterInfoMenu::BackButtonOnClicked);
		BackButton->OnHovered.AddDynamic(this, &UCombatCharacterInfoMenu::BackButtonOnHovered);
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
			if (IsValid(PlayerCharacter->GetUIManagerWorldSubsystem())) {
				if (IsValid(PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton))
					PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
				PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton = BattleMenu->GetAttackTalkInfoActionButton();
				PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButtonIndex = 0;
				PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			BattleMenu->AddToViewport();
			BattleMenu->GetCenterMark()->SetVisibility(ESlateVisibility::Visible);
			BattleMenu->GetEnemyNameBorder()->SetVisibility(ESlateVisibility::Visible);
			BattleMenu->GetLeftRightMenuBorder()->SetVisibility(ESlateVisibility::Visible);
			BattleMenu->GetAttackMenuBorder()->SetVisibility(ESlateVisibility::Visible);
		}
}

void UCombatCharacterInfoMenu::BackButtonOnHovered()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (IsValid(PlayerCharacter->GetUIManagerWorldSubsystem())) {
			if (IsValid(PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton))
				PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
			PlayerCharacter->GetUIManagerWorldSubsystem()->PickedButton = BackButton;
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

UScrollBox* UCombatCharacterInfoMenu::GetActiveEffectsScrollBox() const
{
	return ActiveEffectsScrollBox;
}

UButton* UCombatCharacterInfoMenu::GetBackButton() const
{
	return BackButton;
}

UBorder* UCombatCharacterInfoMenu::GetEffectInfoBorder() const
{
	return EffectInfoBorder;
}
