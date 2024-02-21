// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedSpellTypeEntryWidget.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\Menus\SpellBattleMenu.h"

bool USelectedSpellTypeEntryWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(MainButton)) {
		MainButton->OnClicked.AddDynamic(this, &USelectedSpellTypeEntryWidget::MainButtonOnClicked);
		MainButton->OnHovered.AddDynamic(this, &USelectedSpellTypeEntryWidget::MainButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void USelectedSpellTypeEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USelectedSpellTypeEntryWidget::MainButtonOnClicked()
{
	//Delete this widget and nullify selected spell type
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		if (USpellBattleMenu* SpellBattleMenu = UIManagerWorldSubsystem->SpellBattleMenuWidget; IsValid(SpellBattleMenu)) {
			SpellBattleMenu->SetSelectedSpellType(ESpellType::NONE);
			SpellBattleMenu->GetSelectedSpellTypeHorizontalBox()->RemoveChild(this);
			SpellBattleMenu->SetCreatedSpell(nullptr);
			SpellBattleMenu->ShowSpellTypesButtonsHideElementsAndRangeButtons();
			SpellBattleMenu->CanUseKeyboardButtonSelection = true;
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0.8));
			UIManagerWorldSubsystem->PickedButton = SpellBattleMenu->GetAssaultSpellTypeButtonWithNeighbors();
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
		}
		this->ConditionalBeginDestroy();
}

void USelectedSpellTypeEntryWidget::MainButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		if (USpellBattleMenu* SpellBattleMenu = UIManagerWorldSubsystem->SpellBattleMenuWidget; IsValid(SpellBattleMenu)) {
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
			UIManagerWorldSubsystem->PickedButton = MainButton;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			SpellBattleMenu->CanUseKeyboardButtonSelection = false;
		}
}

void USelectedSpellTypeEntryWidget::SetSpellType(ESpellType NewSpellType)
{
	SpellType = NewSpellType;
}

ESpellType USelectedSpellTypeEntryWidget::GetSpellType() const
{
	return SpellType;
}

UImage* USelectedSpellTypeEntryWidget::GetMainImage() const
{
	return MainImage;
}