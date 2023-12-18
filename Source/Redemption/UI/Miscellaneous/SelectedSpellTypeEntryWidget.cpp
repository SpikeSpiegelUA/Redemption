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
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (USpellBattleMenu* SpellBattleMenu = PlayerCharacter->GetSpellBattleMenuWidget(); IsValid(PlayerCharacter))
		{
			SpellBattleMenu->SetSelectedSpellType(ESpellType::NONE);
			SpellBattleMenu->GetSelectedSpellTypeHorizontalBox()->RemoveChild(this);
			SpellBattleMenu->SetCreatedSpell(nullptr);
			SpellBattleMenu->ShowSpellTypesButtonsHideElementsAndRangeButtons();
			SpellBattleMenu->CanUseKeyboardButtonSelection = true;
			if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = PlayerCharacter->GetUIManagerWorldSubsystem(); IsValid(UIManagerWorldSubsystem)) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0.8));
				UIManagerWorldSubsystem->PickedButton = SpellBattleMenu->GetAssaultSpellTypeButtonWithNeighbors();
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
			}
			this->ConditionalBeginDestroy();
		}
}

void USelectedSpellTypeEntryWidget::MainButtonOnHovered()
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