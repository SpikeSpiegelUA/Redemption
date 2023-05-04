// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\BattleResultsScreen.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Components/StackBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

bool UBattleResultsScreen::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (ContinueButton)
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
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	PlayerCharacter->GetGameManager()->EndBattle();
}

void UBattleResultsScreen::SetAmountOfGoldTextBlock(FText TextToSet)
{
	AmountOfGoldTextBlock->SetText(TextToSet);
}

void UBattleResultsScreen::AddExperienceLevelUpStackBox(UUserWidget* WidgetToAdd)
{
	ExperienceLevelUpStackBox->AddChildToStackBox(WidgetToAdd);
}

UTextBlock* UBattleResultsScreen::GetExperienceTextBlock()
{
	return ExperienceTextBlock;
}

UTextBlock* UBattleResultsScreen::GetGoldTextBlock()
{
	return GoldTextBlock;
}

UTextBlock* UBattleResultsScreen::GetAmountOfGoldTextBlock()
{
	return AmountOfGoldTextBlock;
}

UStackBox* UBattleResultsScreen::GetExperienceLevelUPStackBox()
{
	return ExperienceLevelUpStackBox;
}

UButton* UBattleResultsScreen::GetContinueButton()
{
	return ContinueButton;
}
