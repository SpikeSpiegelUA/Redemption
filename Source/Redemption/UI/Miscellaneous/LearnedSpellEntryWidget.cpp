// Fill out your copyright notice in the Description page of Project Settings.


#include "LearnedSpellEntryWidget.h"
#include "..\Public\UIManagerWorldSubsystem.h"
#include "..\Characters\Player\PlayerCharacter.h"

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
    if (const auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
        if ((IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget) || IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget)) && UIManagerWorldSubsystem->SpellInfoWidget) {
            if(!UIManagerWorldSubsystem->SpellInfoWidget->IsInViewport())
                UIManagerWorldSubsystem->SpellInfoWidget->AddToViewport();
            if (UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport()) {
                UIManagerWorldSubsystem->SpellInfoWidget->SetPositionInViewport(FVector2D(300, 60));
                if (IsValid(UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->SelectedSpellButton))
                    UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->SelectedSpellButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
                UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->SelectedSpellButton = MainButton;
                if (IsValid(UIManagerWorldSubsystem->SpellBattleMenuWidget))
                    UIManagerWorldSubsystem->SpellBattleMenuWidget->SetCreatedSpell(EntrySpell);
            }
            else if (UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport()) {
                UIManagerWorldSubsystem->SpellInfoWidget->SetPositionInViewport(FVector2D(850, 79));
                if (IsValid(UIManagerWorldSubsystem->SkillBattleMenuWidget->SelectedSkillButton))
                    UIManagerWorldSubsystem->SkillBattleMenuWidget->SelectedSkillButton->SetBackgroundColor(FLinearColor(0.6f, 0.6f, 0.6f, 1.f));
                UIManagerWorldSubsystem->SkillBattleMenuWidget->SelectedSkillButton = MainButton;
            }
            UIManagerWorldSubsystem->SpellInfoWidget->SetSpellInfo(EntrySpell);
            MainButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
        }
}

void ULearnedSpellEntryWidget::MainButtonOnHovered()
{
    if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
        if (UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->IsInViewport()) {
            if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
                if (UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->SelectedSpellButton == UIManagerWorldSubsystem->PickedButton)
                    UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                else
                    UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
            }
            UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->CanUseKeyboardButtonSelection = true;
            for (uint8 Index = 0; Index < UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren().Num(); Index++)
                if (UIManagerWorldSubsystem->LearnedSpellsJournalMenuWidget->GetMainScrollBox()->GetAllChildren()[Index] == this) {
                    UIManagerWorldSubsystem->PickedButtonIndex = Index;
                    break;
                }
        }
        else if (UIManagerWorldSubsystem->SkillBattleMenuWidget->IsInViewport()) {
            if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
                if (UIManagerWorldSubsystem->SkillBattleMenuWidget->SelectedSkillButton == UIManagerWorldSubsystem->PickedButton)
                    UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                else
                    UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
            }
            UIManagerWorldSubsystem->SkillBattleMenuWidget->CanUseKeyboardButtonSelection = true;
            for (uint8 Index = 0; Index < UIManagerWorldSubsystem->SkillBattleMenuWidget->GetSkillsScrollBox()->GetAllChildren().Num(); Index++)
                if (UIManagerWorldSubsystem->SkillBattleMenuWidget->GetSkillsScrollBox()->GetAllChildren()[Index] == this) {
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
