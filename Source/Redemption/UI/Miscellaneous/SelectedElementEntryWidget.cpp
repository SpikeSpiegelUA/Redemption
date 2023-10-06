// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\SelectedElementEntryWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SpellBattleMenu.h"

bool USelectedElementEntryWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if(IsValid(MainButton))
		MainButton->OnClicked.AddDynamic(this, &USelectedElementEntryWidget::MainButtonOnClicked);
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
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) 
		if (USpellBattleMenu* SkillBattleMenu = PlayerCharacter->GetSpellBattleMenuWidget(); IsValid(PlayerCharacter)) 
			for (UWidget* Child : SkillBattleMenu->GetSelectedElementsHorizontalBox()->GetAllChildren()) 
				if(USelectedElementEntryWidget* SelectedElementEntryWidget = Cast<USelectedElementEntryWidget>(Child); IsValid(SelectedElementEntryWidget))
					if (Child == this) {
						SkillBattleMenu->SelectedSpellElementsRemoveSingleItem(Element);
						SkillBattleMenu->GetSelectedElementsHorizontalBox()->RemoveChild(Child);
						if(IsValid(Child))
							Child->ConditionalBeginDestroy();
						SkillBattleMenu->SetCreatedSpell(nullptr);
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
