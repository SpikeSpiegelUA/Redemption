// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveEffectEntryWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Public\UIManagerWorldSubsystem.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

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
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())) {
        if (IsValid(PlayerCharacter->GetCombatCharacterInfoMenuWidget()->SelectedEffectButton))
            PlayerCharacter->GetCombatCharacterInfoMenuWidget()->SelectedEffectButton->SetBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1));
        PlayerCharacter->GetCombatCharacterInfoMenuWidget()->SelectedEffectButton = MainButton;
        PlayerCharacter->GetCombatCharacterInfoMenuWidget()->SetEffectInfo(EntryEffect);
        MainButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
    }
}

void UActiveEffectEntryWidget::MainButtonOnHovered()
{
    if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
        if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())) {
            if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
                if (PlayerCharacter->GetCombatCharacterInfoMenuWidget()->SelectedEffectButton == UIManagerWorldSubsystem->PickedButton)
                    UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                else
                    UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
            }
            PlayerCharacter->GetCombatCharacterInfoMenuWidget()->CanUseKeyboardButtonSelection = true;
            for (uint8 Index = 0; Index < PlayerCharacter->GetCombatCharacterInfoMenuWidget()->GetActiveEffectsScrollBox()->GetAllChildren().Num(); Index++)
                if (PlayerCharacter->GetCombatCharacterInfoMenuWidget()->GetActiveEffectsScrollBox()->GetAllChildren()[Index] == this) {
                    UIManagerWorldSubsystem->PickedButtonIndex = Index;
                    break;
                }
            UIManagerWorldSubsystem->PickedButton = MainButton;
            UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
        }
}

void UActiveEffectEntryWidget::SetEffectTypeImage(const UTexture* const ImageTexture)
{
    EffectTypeImage->Brush.SetResourceObject(const_cast<UTexture*>(ImageTexture));
    EffectTypeImage->Brush.SetImageSize(FVector2D(200, 200));
}

void UActiveEffectEntryWidget::SetEffectAreaImage(const UTexture* const ImageTexture)
{
    EffectAreaImage->Brush.SetResourceObject(const_cast<UTexture*>(ImageTexture));
    EffectAreaImage->Brush.SetImageSize(FVector2D(200, 200));
}

void UActiveEffectEntryWidget::SetEffectNameText(const FText& NewText)
{
    EffectNameTextBlock->SetText(NewText);
}

UButton* UActiveEffectEntryWidget::GetMainButton() const
{
    return MainButton;
}
