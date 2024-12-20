// Fill out your copyright notice in the Description page of Project Settings.


#include "..\UI\Miscellaneous\SelectedElementEntryWidget.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\Menus\SpellBattleMenu.h"

bool USelectedElementEntryWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(MainButton)) {
		MainButton->OnClicked.AddDynamic(this, &USelectedElementEntryWidget::MainButtonOnClicked);
		MainButton->OnHovered.AddDynamic(this, &USelectedElementEntryWidget::MainButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void USelectedElementEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USelectedElementEntryWidget::MainButtonOnClicked()
{
	//Delete this widget and decrease selected elements count on click
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		if (USpellBattleMenu* SpellBattleMenu = UIManagerWorldSubsystem->SpellBattleMenuWidget; IsValid(UIManagerWorldSubsystem)) {
			for (int8 Index = SpellBattleMenu->GetSelectedElementsHorizontalBox()->GetAllChildren().Num() - 1; Index >= 0; Index--)
				if (USelectedElementEntryWidget* SelectedElementEntryWidget = Cast<USelectedElementEntryWidget>(SpellBattleMenu->GetSelectedElementsHorizontalBox()->GetAllChildren()[Index]); IsValid(SelectedElementEntryWidget))
					if (SpellBattleMenu->GetSelectedElementsHorizontalBox()->GetAllChildren()[Index] == this) {
						SpellBattleMenu->SelectedSpellElementsRemoveSingleItem(Element);
						SpellBattleMenu->GetSelectedElementsHorizontalBox()->RemoveChildAt(Index);
						SpellBattleMenu->SetCreatedSpell(nullptr);
					}
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 0.8));
			UIManagerWorldSubsystem->PickedButton = SpellBattleMenu->GetWaterElementButton();
			SpellBattleMenu->CanUseKeyboardButtonSelection = true;
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 0.8));
		}

}

void USelectedElementEntryWidget::MainButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		if (USpellBattleMenu* SpellBattleMenu = UIManagerWorldSubsystem->SpellBattleMenuWidget; IsValid(UIManagerWorldSubsystem)){
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3, 0.3, 0.3, 1));
			UIManagerWorldSubsystem->PickedButton = MainButton;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			SpellBattleMenu->CanUseKeyboardButtonSelection = false;
		}
}

void USelectedElementEntryWidget::SetElement(ESpellElements NewElement)
{
	Element = NewElement;
}

ESpellElements USelectedElementEntryWidget::GetElement() const
{
	return Element;
}

UImage* USelectedElementEntryWidget::GetMainImage() const
{
	return MainImage;
}
