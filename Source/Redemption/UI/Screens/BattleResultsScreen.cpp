// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\BattleResultsScreen.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Components/StackBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UBattleResultsScreen::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(ContinueButton))
		ContinueButton->OnClicked.AddDynamic(this, &UBattleResultsScreen::ContinueButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void UBattleResultsScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBattleResultsScreen::ContinueButtonOnClicked()
{
	if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		PlayerCharacter->GetGameManager()->EndBattle();
}

void UBattleResultsScreen::SetAmountOfGoldTextBlock(const FText& TextToSet)
{
	AmountOfGoldTextBlock->SetText(TextToSet);
}

void UBattleResultsScreen::AddExperienceLevelUpStackBox(UUserWidget* WidgetToAdd)
{
	ExperienceLevelUpStackBox->AddChildToStackBox(WidgetToAdd);
}

UTextBlock* UBattleResultsScreen::GetExperienceTextBlock() const
{
	return ExperienceTextBlock;
}

UTextBlock* UBattleResultsScreen::GetGoldTextBlock() const
{
	return GoldTextBlock;
}

UTextBlock* UBattleResultsScreen::GetAmountOfGoldTextBlock() const
{
	return AmountOfGoldTextBlock;
}

UStackBox* UBattleResultsScreen::GetExperienceLevelUPStackBox() const
{
	return ExperienceLevelUpStackBox;
}

UButton* UBattleResultsScreen::GetContinueButton() const
{
	return ContinueButton;
}
