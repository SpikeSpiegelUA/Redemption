// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\LearnedSpellsJournalMenu.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ElementsActions.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

bool ULearnedSpellsJournalMenu::Initialize()
{
    const bool bSuccess = Super::Initialize();
    if (IsValid(BackButton)) {
        BackButton->OnClicked.AddDynamic(this, &ULearnedSpellsJournalMenu::BackButtonOnClicked);
        BackButton->OnHovered.AddDynamic(this, &ULearnedSpellsJournalMenu::BackButtonOnHovered);
    }
    if (IsValid(UseButton)) {
        UseButton->OnClicked.AddDynamic(this, &ULearnedSpellsJournalMenu::UseButtonOnClicked);
        UseButton->OnHovered.AddDynamic(this, &ULearnedSpellsJournalMenu::UseButtonOnHovered);
    }
    if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController()))
        PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
    if (!bSuccess) return false;
    return bSuccess;
}

void ULearnedSpellsJournalMenu::NativeConstruct()
{
    Super::NativeConstruct();
}

void ULearnedSpellsJournalMenu::UseButtonOnClicked()
{
    if (IsValid(PlayerCharacter)) {
        PlayerCharacter->GetSpellBattleMenuWidget()->UseUniqueSpellButtonOnClicked();
        this->RemoveFromParent();
    }
}

void ULearnedSpellsJournalMenu::BackButtonOnClicked()
{
    this->RemoveFromParent();
    if (IsValid(PlayerCharacter)) {
        PlayerCharacter->GetSpellBattleMenuWidget()->ShowSpellBattleMenu();
        PlayerCharacter->GetAlliesInfoBarsWidget()->AddToViewport(-1);
        PlayerCharacter->GetSpellBattleMenuWidget()->GetSpellInfoBorder()->SetVisibility(ESlateVisibility::Hidden);
        PlayerCharacter->GetSpellBattleMenuWidget()->CanUseKeyboardButtonSelection = true;
        if (IsValid(PlayerCharacter->GetBattleMenuWidget()))
            PlayerCharacter->GetBattleMenuWidget()->IsChoosingLearnedSpell = false;
        if(IsValid(PlayerCharacter->GetSpellBattleMenuWidget()))
            PlayerCharacter->GetSpellBattleMenuWidget()->GetSpellInfoBorder()->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULearnedSpellsJournalMenu::UseButtonOnHovered()
{
    if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
        if (UIManagerWorldSubsystem->PickedButton)
            UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
        UIManagerWorldSubsystem->PickedButton = UseButton;
        UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
    }
}

void ULearnedSpellsJournalMenu::BackButtonOnHovered()
{
    if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
        if (UIManagerWorldSubsystem->PickedButton)
            UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
        UIManagerWorldSubsystem->PickedButton = BackButton;
        UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
    }
}

void ULearnedSpellsJournalMenu::AddLearnedSpellEntryToMainScrollBox(const class ASpell* const SpellToAdd)
{
    if(IsValid(LearnedSpellEntryWidgetClass))
        LearnedSpellEntryWidget = CreateWidget<ULearnedSpellEntryWidget>(GetWorld(), LearnedSpellEntryWidgetClass);
    if (IsValid(PlayerCharacter) && IsValid(LearnedSpellEntryWidget)) {
        LearnedSpellEntryWidget->SetSpellTypeImage(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetSpellTypeImageTexture(SpellToAdd->GetTypeOfSpell()));
        LearnedSpellEntryWidget->SetSpellMainElementImage(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetMainSpellElementImageTexture(ElementsActions::FindSpellsMainElement(SpellToAdd->GetSpellElements())));
        LearnedSpellEntryWidget->SetSpellNameText(SpellToAdd->GetSpellName());
        LearnedSpellEntryWidget->EntrySpell = const_cast<ASpell*>(SpellToAdd);
        MainScrollBox->AddChild(LearnedSpellEntryWidget);
    } 
}

UScrollBox* ULearnedSpellsJournalMenu::GetMainScrollBox() const
{
    return MainScrollBox;
}

UButton* ULearnedSpellsJournalMenu::GetUseButton() const
{
    return UseButton;
}
