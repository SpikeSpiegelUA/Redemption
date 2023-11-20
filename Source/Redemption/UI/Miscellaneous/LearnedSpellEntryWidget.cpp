// Fill out your copyright notice in the Description page of Project Settings.


#include "LearnedSpellEntryWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Public\UIManagerWorldSubsystem.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

bool ULearnedSpellEntryWidget::Initialize()
{
    const bool bSuccess = Super::Initialize();
    if (IsValid(MainButton)) {
        MainButton->OnClicked.AddDynamic(this, &ULearnedSpellEntryWidget::MainButtonOnClicked);
        MainButton->OnHovered.AddDynamic(this, &ULearnedSpellEntryWidget::MainButtonOnHovered);
    }
    if (!bSuccess) return false;
    return bSuccess;
}

void ULearnedSpellEntryWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void ULearnedSpellEntryWidget::MainButtonOnClicked()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())) {
        PlayerCharacter->GetSpellInfoWidget()->AddToViewport();
        if (PlayerCharacter->GetLearnedSpellsJournalMenu()->IsInViewport()) {
            PlayerCharacter->GetSpellInfoWidget()->SetPositionInViewport(FVector2D(300, 60));
            if (IsValid(PlayerCharacter->GetLearnedSpellsJournalMenu()->SelectedSpellButton))
                PlayerCharacter->GetLearnedSpellsJournalMenu()->SelectedSpellButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
            PlayerCharacter->GetLearnedSpellsJournalMenu()->SelectedSpellButton = MainButton;
        }
        else if (PlayerCharacter->GetSkillBattleMenuWidget()->IsInViewport()) {
            PlayerCharacter->GetSpellInfoWidget()->SetPositionInViewport(FVector2D(720, 20));
            if (IsValid(PlayerCharacter->GetSkillBattleMenuWidget()->SelectedSkillButton))
                PlayerCharacter->GetSkillBattleMenuWidget()->SelectedSkillButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
            PlayerCharacter->GetSkillBattleMenuWidget()->SelectedSkillButton = MainButton;
        }
        PlayerCharacter->GetSpellBattleMenuWidget()->SetCreatedSpell(EntrySpell);
        PlayerCharacter->GetSpellInfoWidget()->SetSpellInfo(EntrySpell);
        MainButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
    }
}

void ULearnedSpellEntryWidget::MainButtonOnHovered()
{
    if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
        if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())) {
            if (PlayerCharacter->GetLearnedSpellsJournalMenu()->IsInViewport()) {
                if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
                    if (PlayerCharacter->GetLearnedSpellsJournalMenu()->SelectedSpellButton == UIManagerWorldSubsystem->PickedButton)
                        UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                    else
                        UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
                }
                PlayerCharacter->GetLearnedSpellsJournalMenu()->CanUseKeyboardButtonSelection = true;
                for (uint8 Index = 0; Index < PlayerCharacter->GetLearnedSpellsJournalMenu()->GetMainScrollBox()->GetAllChildren().Num(); Index++)
                    if (PlayerCharacter->GetLearnedSpellsJournalMenu()->GetMainScrollBox()->GetAllChildren()[Index] == this) {
                        UIManagerWorldSubsystem->PickedButtonIndex = Index;
                        break;
                    }
            }
            else if (PlayerCharacter->GetSkillBattleMenuWidget()->IsInViewport()) {
                if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
                    if (PlayerCharacter->GetSkillBattleMenuWidget()->SelectedSkillButton == UIManagerWorldSubsystem->PickedButton)
                        UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                    else
                        UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
                }
                PlayerCharacter->GetSkillBattleMenuWidget()->CanUseKeyboardButtonSelection = true;
                for (uint8 Index = 0; Index < PlayerCharacter->GetSkillBattleMenuWidget()->GetSkillsScrollBox()->GetAllChildren().Num(); Index++)
                    if (PlayerCharacter->GetSkillBattleMenuWidget()->GetSkillsScrollBox()->GetAllChildren()[Index] == this) {
                        UIManagerWorldSubsystem->PickedButtonIndex = Index;
                        break;
                    }
            }
            UIManagerWorldSubsystem->PickedButton = MainButton;
            UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
        }
}

void ULearnedSpellEntryWidget::SetSpellTypeImage(const UTexture* const ImageTexture)
{
    SpellTypeImage->Brush.SetResourceObject(const_cast<UTexture*>(ImageTexture));
    SpellTypeImage->Brush.SetImageSize(FVector2D(200, 200));
}

void ULearnedSpellEntryWidget::SetSpellMainElementImage(const UTexture* const ImageTexture)
{
    SpellMainElementImage->Brush.SetResourceObject(const_cast<UTexture*>(ImageTexture));
    SpellMainElementImage->Brush.SetImageSize(FVector2D(200, 200));
}

void ULearnedSpellEntryWidget::SetSpellNameText(const FText& NewText)
{
    SpellNameTextBlock->SetText(NewText);
}

UButton* ULearnedSpellEntryWidget::GetMainButton() const
{
    return MainButton;
}
