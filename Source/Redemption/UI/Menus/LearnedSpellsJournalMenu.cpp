// Fill out your copyright notice in the Description page of Project Settings.


#include "..\UI\Menus\LearnedSpellsJournalMenu.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"

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
    if (auto* RedemptionGameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(RedemptionGameInstance) && IsValid(PlayerCharacter))
        for (TSubclassOf<ASpell> InstanceSpellClass : RedemptionGameInstance->InstanceLearnedSpells)
            if (IsValid(Cast<ASpell>(InstanceSpellClass->GetDefaultObject())))
                AddLearnedSpellEntryToMainScrollBox(Cast<ASpell>(InstanceSpellClass->GetDefaultObject()));
    if (!bSuccess) return false;
    return bSuccess;
}

void ULearnedSpellsJournalMenu::NativeConstruct()
{
    Super::NativeConstruct();
}

void ULearnedSpellsJournalMenu::UseButtonOnClicked()
{
    if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) 
        if (ASpell* CreatedSpell = UIManagerWorldSubsystem->SpellBattleMenuWidget->GetCreatedSpell(); IsValid(CreatedSpell))
            if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
                if (ABattleManager* BManager = RedemptionGameModeBase->GetBattleManager(); IsValid(BManager)) {
                    float VariableCorrespondingToSpellCostType{};
                    if (CreatedSpell->GetSpellCostType() == ESpellCostType::MANA)
                        VariableCorrespondingToSpellCostType = BManager->BattleAlliesPlayer[BManager->CurrentTurnCombatNPCIndex]->CurrentMana;
                    else
                        VariableCorrespondingToSpellCostType = BManager->BattleAlliesPlayer[BManager->CurrentTurnCombatNPCIndex]->CurrentHP;
                    if ((CreatedSpell->GetSpellCostType() == ESpellCostType::MANA && VariableCorrespondingToSpellCostType >= CreatedSpell->GetCost()) ||
                        (CreatedSpell->GetSpellCostType() == ESpellCostType::HEALTH && VariableCorrespondingToSpellCostType > CreatedSpell->GetCost())) {
                        UIManagerWorldSubsystem->SpellBattleMenuWidget->UseSpell(true);
                        this->RemoveFromParent();
                        if (UIManagerWorldSubsystem->SpellInfoWidget->IsInViewport())
                            UIManagerWorldSubsystem->SpellInfoWidget->RemoveFromParent();
                        UIManagerWorldSubsystem->AlliesInfoBarsWidget->AddToViewport();
                    }
                    else if (CreatedSpell->GetSpellCostType() == ESpellCostType::MANA && VariableCorrespondingToSpellCostType < CreatedSpell->GetCost())
                        ActivateNotification(FText::FromString("Not enough mana!"));
                    else if(CreatedSpell->GetSpellCostType() == ESpellCostType::HEALTH && VariableCorrespondingToSpellCostType > CreatedSpell->GetCost())
                        ActivateNotification(FText::FromString("Not enough health!"));
                }
}

void ULearnedSpellsJournalMenu::BackButtonOnClicked()
{
    this->RemoveFromParent();
    if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
        UIManagerWorldSubsystem->SpellBattleMenuWidget->AddToViewport();
        UIManagerWorldSubsystem->SpellBattleMenuWidget->OnMenuOpenUIManagerLogic();
        UIManagerWorldSubsystem->AlliesInfoBarsWidget->AddToViewport(-1);
        UIManagerWorldSubsystem->SpellBattleMenuWidget->CanUseKeyboardButtonSelection = true;
        if (IsValid(UIManagerWorldSubsystem->BattleMenuWidget))
            UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingLearnedSpell = false;
        SelectedSpellButton = nullptr;
        if (UIManagerWorldSubsystem->SpellInfoWidget->IsInViewport())
            UIManagerWorldSubsystem->SpellInfoWidget->RemoveFromParent();
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
        UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->CanUseKeyboardButtonSelection = false;
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
            UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->CanUseKeyboardButtonSelection = false;
    }
}

void ULearnedSpellsJournalMenu::AddLearnedSpellEntryToMainScrollBox(const class ASpell* const SpellToAdd)
{
    if(IsValid(LearnedSpellEntryWidgetClass))
        LearnedSpellEntryWidget = CreateWidget<ULearnedSpellEntryWidget>(GetWorld(), LearnedSpellEntryWidgetClass);
    if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
        if (IsValid(LearnedSpellEntryWidget) && IsValid(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager())) {
            LearnedSpellEntryWidget->SetSpellTypeImage(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetSpellTypeImageTexture(SpellToAdd->GetTypeOfSpell()));
            LearnedSpellEntryWidget->SetSpellMainElementImage(RedemptionGameModeBase->GetEffectsSpellsAndSkillsManager()->GetMainSpellElementImageTexture(ElementsActions::FindSpellsMainElement(SpellToAdd->GetSpellElements())));
            LearnedSpellEntryWidget->SetSpellNameText(SpellToAdd->GetSpellName());
            LearnedSpellEntryWidget->EntrySpell = const_cast<ASpell*>(SpellToAdd);
            MainScrollBox->AddChild(LearnedSpellEntryWidget);
        }
}

void ULearnedSpellsJournalMenu::ActivateNotification(const FText& NotificationText)
{
    NotificationTextBlock->SetText(NotificationText);
    PlayAnimation(NotificationShowAndHide);
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
