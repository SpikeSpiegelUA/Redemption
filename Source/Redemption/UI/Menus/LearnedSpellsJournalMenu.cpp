// Fill out your copyright notice in the Description page of Project Settings.


#include "..\UI\Menus\LearnedSpellsJournalMenu.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

bool ULearnedSpellsJournalMenu::Initialize()
{
    const bool bSuccess = Super::Initialize();
    if (IsValid(BackButtonWithNeighbors)) {
        BackButtonWithNeighbors->OnClicked.AddDynamic(this, &ULearnedSpellsJournalMenu::BackButtonOnClicked);
        BackButtonWithNeighbors->OnHovered.AddDynamic(this, &ULearnedSpellsJournalMenu::BackButtonOnHovered);
    }
    if (IsValid(UseButtonWithNeighbors)) {
        UseButtonWithNeighbors->OnClicked.AddDynamic(this, &ULearnedSpellsJournalMenu::UseButtonOnClicked);
        UseButtonWithNeighbors->OnHovered.AddDynamic(this, &ULearnedSpellsJournalMenu::UseButtonOnHovered);
    }
    if (IsValid(GetWorld()) && IsValid(GetWorld()->GetFirstPlayerController()))
        PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

    if (!bSuccess) return false;
    return bSuccess;
}

void ULearnedSpellsJournalMenu::NativeConstruct()
{
    Super::NativeConstruct();
    if (IsValid(PlayerCharacter))
        for (TSubclassOf<ASpell> InstanceSpellClass : PlayerCharacter->GetGameInstance()->InstanceLearnedSpells)
            if (IsValid(Cast<ASpell>(InstanceSpellClass->GetDefaultObject())))
                AddLearnedSpellEntryToMainScrollBox(Cast<ASpell>(InstanceSpellClass->GetDefaultObject()));
}

void ULearnedSpellsJournalMenu::UseButtonOnClicked()
{
    if (IsValid(PlayerCharacter)) 
        if (ASpell* CreatedSpell = PlayerCharacter->GetSpellBattleMenuWidget()->GetCreatedSpell(); IsValid(CreatedSpell)) 
            if (ABattleManager* BManager = PlayerCharacter->GetBattleManager(); IsValid(BManager)) {
                float VariableCorrespondingToSpellCostType{};
                if (CreatedSpell->GetSpellCostType() == ESpellCostType::MANA)
                    VariableCorrespondingToSpellCostType = BManager->BattleAlliesPlayer[BManager->CurrentTurnCombatNPCIndex]->CurrentMana;
                else
                    VariableCorrespondingToSpellCostType = BManager->BattleAlliesPlayer[BManager->CurrentTurnCombatNPCIndex]->CurrentHP;
                if ((CreatedSpell->GetSpellCostType() == ESpellCostType::MANA && VariableCorrespondingToSpellCostType >= CreatedSpell->GetCost()) ||
                    (CreatedSpell->GetSpellCostType() == ESpellCostType::HEALTH && VariableCorrespondingToSpellCostType > CreatedSpell->GetCost())) {
                    PlayerCharacter->GetSpellBattleMenuWidget()->UseSpell();
                    this->RemoveFromParent();
                    if (PlayerCharacter->GetSpellInfoWidget()->IsInViewport())
                        PlayerCharacter->GetSpellInfoWidget()->RemoveFromParent();
                    PlayerCharacter->GetAlliesInfoBarsWidget()->AddToViewport();
                }
                else if (CreatedSpell->GetSpellCostType() == ESpellCostType::MANA && VariableCorrespondingToSpellCostType < CreatedSpell->GetCost())
                    CreateNotification(FText::FromString("Not enough mana!"));
                else if(CreatedSpell->GetSpellCostType() == ESpellCostType::HEALTH && VariableCorrespondingToSpellCostType > CreatedSpell->GetCost())
                    CreateNotification(FText::FromString("Not enough health!"));
            }
}

void ULearnedSpellsJournalMenu::BackButtonOnClicked()
{
    this->RemoveFromParent();
    if (IsValid(PlayerCharacter)) {
        PlayerCharacter->GetSpellBattleMenuWidget()->AddToViewport();
        PlayerCharacter->GetSpellBattleMenuWidget()->OnMenuOpenUIManagerLogic();
        PlayerCharacter->GetAlliesInfoBarsWidget()->AddToViewport(-1);
        PlayerCharacter->GetSpellBattleMenuWidget()->CanUseKeyboardButtonSelection = true;
        if (IsValid(PlayerCharacter->GetBattleMenuWidget()))
            PlayerCharacter->GetBattleMenuWidget()->IsChoosingLearnedSpell = false;
        SelectedSpellButton = nullptr;
        if (PlayerCharacter->GetSpellInfoWidget()->IsInViewport())
            PlayerCharacter->GetSpellInfoWidget()->RemoveFromParent();
    }
}

void ULearnedSpellsJournalMenu::UseButtonOnHovered()
{
    if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
        if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
            if(SelectedSpellButton == UIManagerWorldSubsystem->PickedButton)
                UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
            else
                UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
        }
        UIManagerWorldSubsystem->PickedButton = UseButtonWithNeighbors;
        UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
        if(IsValid(PlayerCharacter))
            PlayerCharacter->GetLearnedSpellsJournalMenu()->CanUseKeyboardButtonSelection = false;
    }
}

void ULearnedSpellsJournalMenu::BackButtonOnHovered()
{
    if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
        if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
            if (SelectedSpellButton == UIManagerWorldSubsystem->PickedButton)
                UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
            else
                UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
        }
        UIManagerWorldSubsystem->PickedButton = BackButtonWithNeighbors;
        UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
        if (IsValid(PlayerCharacter))
            PlayerCharacter->GetLearnedSpellsJournalMenu()->CanUseKeyboardButtonSelection = false;
    }
}

void ULearnedSpellsJournalMenu::AddLearnedSpellEntryToMainScrollBox(const class ASpell* const SpellToAdd)
{
    if(IsValid(LearnedSpellEntryWidgetClass))
        LearnedSpellEntryWidget = CreateWidget<ULearnedSpellEntryWidget>(GetWorld(), LearnedSpellEntryWidgetClass);
    if (IsValid(PlayerCharacter) && IsValid(LearnedSpellEntryWidget) && IsValid(PlayerCharacter->GetEffectsSpellsAndSkillsManager())) {
        LearnedSpellEntryWidget->SetSpellTypeImage(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetSpellTypeImageTexture(SpellToAdd->GetTypeOfSpell()));
        LearnedSpellEntryWidget->SetSpellMainElementImage(PlayerCharacter->GetEffectsSpellsAndSkillsManager()->GetMainSpellElementImageTexture(ElementsActions::FindSpellsMainElement(SpellToAdd->GetSpellElements())));
        LearnedSpellEntryWidget->SetSpellNameText(SpellToAdd->GetSpellName());
        LearnedSpellEntryWidget->EntrySpell = const_cast<ASpell*>(SpellToAdd);
        MainScrollBox->AddChild(LearnedSpellEntryWidget);
    }
}

void ULearnedSpellsJournalMenu::CreateNotification(const FText& NotificationText)
{
    NotificationBorder->SetVisibility(ESlateVisibility::Visible);
    NotificationTextBlock->SetText(NotificationText);
    GetWorld()->GetTimerManager().SetTimer(HideNotificationTimerHandle, this, &ULearnedSpellsJournalMenu::HideNotificationAndClearItsTimer, 3.f, false);
}

void ULearnedSpellsJournalMenu::HideNotificationAndClearItsTimer()
{
    NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
    NotificationTextBlock->SetText(FText::FromString(""));
    GetWorld()->GetTimerManager().ClearTimer(HideNotificationTimerHandle);
}

const UScrollBox* ULearnedSpellsJournalMenu::GetMainScrollBox() const
{
    return MainScrollBox;
}

UButtonWithNeighbors* ULearnedSpellsJournalMenu::GetUseButtonWithNeighbors() const
{
    return UseButtonWithNeighbors;
}

UButtonWithNeighbors* ULearnedSpellsJournalMenu::GetBackButtonWithNeighbors() const
{
    return BackButtonWithNeighbors;
}
