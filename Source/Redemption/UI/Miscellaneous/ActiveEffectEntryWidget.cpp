// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveEffectEntryWidget.h"
#include "..\Public\UIManagerWorldSubsystem.h"
#include "..\Characters\Player\PlayerCharacter.h"

bool UActiveEffectEntryWidget::Initialize()
{
    const bool bSuccess = Super::Initialize();
    if (IsValid(MainButton)) {
        MainButton->OnClicked.AddDynamic(this, &UActiveEffectEntryWidget::MainButtonOnClicked);
        MainButton->OnHovered.AddDynamic(this, &UActiveEffectEntryWidget::MainButtonOnHovered);
    }
    if (!bSuccess) return false;
    return bSuccess;
}

void UActiveEffectEntryWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UActiveEffectEntryWidget::MainButtonOnClicked()
{
    if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
        if (IsValid(UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->SelectedEffectButton))
            UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->SelectedEffectButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
        UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->SelectedEffectButton = MainButton;
        UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->SetEffectInfo(EntryEffect);
        MainButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
    }
}

void UActiveEffectEntryWidget::MainButtonOnHovered()
{
    if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
        if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
            if (UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->SelectedEffectButton == UIManagerWorldSubsystem->PickedButton)
                UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
            else
                UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
        }
        UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->CanUseKeyboardButtonSelection = true;
        for (uint8 Index = 0; Index < UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->GetActiveEffectsScrollBox()->GetAllChildren().Num(); Index++)
            if (UIManagerWorldSubsystem->CombatCharacterInfoMenuWidget->GetActiveEffectsScrollBox()->GetAllChildren()[Index] == this) {
                UIManagerWorldSubsystem->PickedButtonIndex = Index;
                break;
            }
        UIManagerWorldSubsystem->PickedButton = MainButton;
        UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
    }
}

void UActiveEffectEntryWidget::SetEffectTypeImage(const UTexture* const ImageTexture)
{
    FSlateBrush NewSlateBrush{};
    NewSlateBrush.SetResourceObject(const_cast<UTexture*>(ImageTexture));
    NewSlateBrush.SetImageSize(FVector2D(200, 200));
    EffectTypeImage->SetBrush(NewSlateBrush);
}

void UActiveEffectEntryWidget::SetEffectAreaImage(const UTexture* const ImageTexture)
{
    FSlateBrush NewSlateBrush{};
    NewSlateBrush.SetResourceObject(const_cast<UTexture*>(ImageTexture));
    NewSlateBrush.SetImageSize(FVector2D(200, 200));
    EffectAreaImage->SetBrush(NewSlateBrush);
}

void UActiveEffectEntryWidget::SetEffectNameText(const FText& NewText)
{
    EffectNameTextBlock->SetText(NewText);
}

UButton* UActiveEffectEntryWidget::GetMainButton() const
{
    return MainButton;
}
