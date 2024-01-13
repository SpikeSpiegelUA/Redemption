// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedSpellRangeEntryWidget.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\Menus\SpellBattleMenu.h"

bool USelectedSpellRangeEntryWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(MainButton)) {
		MainButton->OnClicked.AddDynamic(this, &USelectedSpellRangeEntryWidget::MainButtonOnClicked);
		MainButton->OnHovered.AddDynamic(this, &USelectedSpellRangeEntryWidget::MainButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void USelectedSpellRangeEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USelectedSpellRangeEntryWidget::MainButtonOnClicked()
{
	//Delete this widget and nullify selected spell type
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (USpellBattleMenu* SpellBattleMenu = PlayerCharacter->GetSpellBattleMenuWidget(); IsValid(PlayerCharacter))
		{
			SpellBattleMenu->SetSelectedSpellRange(ESpellRange::NONE);
			SpellBattleMenu->GetSelectedSpellRangeHorizontalBox()->RemoveChild(this);
			SpellBattleMenu->SetCreatedSpell(nullptr);
			SpellBattleMenu->ShowRangeButtonsHideSpellTypesAndElementsButtons();
			SpellBattleMenu->CanUseKeyboardButtonSelection = true;
			if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = PlayerCharacter->GetUIManagerWorldSubsystem(); IsValid(UIManagerWorldSubsystem)) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0.8));
				UIManagerWorldSubsystem->PickedButton = SpellBattleMenu->GetNeighborsSpellRangeButton();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
			}
			this->ConditionalBeginDestroy();
		}
}

void USelectedSpellRangeEntryWidget::MainButtonOnHovered()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (USpellBattleMenu* SpellBattleMenu = PlayerCharacter->GetSpellBattleMenuWidget(); IsValid(PlayerCharacter))
			if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = Cast<UUIManagerWorldSubsystem>(PlayerCharacter->GetUIManagerWorldSubsystem()); IsValid(UIManagerWorldSubsystem)) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
				UIManagerWorldSubsystem->PickedButton = MainButton;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				SpellBattleMenu->CanUseKeyboardButtonSelection = false;
			}
}

void USelectedSpellRangeEntryWidget::SetSpellRange(ESpellRange NewSpellRange)
{
	SpellRange = NewSpellRange;
}

ESpellRange USelectedSpellRangeEntryWidget::GetSpellRange() const
{
	return SpellRange;
}

UImage* USelectedSpellRangeEntryWidget::GetMainImage() const
{
	return MainImage;
}