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
        PlayerCharacter->GetSpellBattleMenuWidget()->SetCreatedSpell(EntrySpell);
        PlayerCharacter->GetSpellBattleMenuWidget()->SetSpellInfo(EntrySpell);
        PlayerCharacter->GetSpellBattleMenuWidget()->GetSpellInfoBorder()->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULearnedSpellEntryWidget::MainButtonOnHovered()
{
    if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
        if(UIManagerWorldSubsystem->PickedButton)
            UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
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
