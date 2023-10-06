// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedSpellTypeEntryWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\SpellBattleMenu.h"

bool USelectedSpellTypeEntryWidget::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(MainButton))
		MainButton->OnClicked.AddDynamic(this, &USelectedSpellTypeEntryWidget::MainButtonOnClicked);
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
		if (USpellBattleMenu* SkillBattleMenu = PlayerCharacter->GetSpellBattleMenuWidget(); IsValid(PlayerCharacter))
		{
			SkillBattleMenu->SetSelectedSpellType(ESpellType::NONE);
			SkillBattleMenu->GetSelectedSpellTypeHorizontalBox()->RemoveChild(this);
			this->ConditionalBeginDestroy();
			SkillBattleMenu->SetCreatedSpell(nullptr);
			SkillBattleMenu->ShowSpellTypesButtonsHideElementsButtons();
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