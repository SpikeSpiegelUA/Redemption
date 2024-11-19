// Fill out your copyright notice in the Description page of Project Settings.


#include "StartFinishGameScreen.h"
#include "Kismet/GameplayStatics.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Redemption/Dynamics/Miscellaneous/QuestAndItsStage.h"
#include "Redemption/Dynamics/Gameplay/Managers/QuestManager.h"
#include "LoadingScreen.h"
#include "UIManagerWorldSubsystem.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UStartFinishGameScreen::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(ContinueButton))
		ContinueButton->OnClicked.AddDynamic(this, &UStartFinishGameScreen::ContinueButtonOnClicked);
	if (!bSuccess) return false;
	return bSuccess;
}

void UStartFinishGameScreen::NativeConstruct()
{
	Super::NativeConstruct();
	OnWidgetShowLogic();
	//Need this for PickedButton, cause if I set PickedButton in NativeConstruct, it will open a MainMenu when triggering the EndGameActor.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UStartFinishGameScreen::PostNativeConstruct);
}

void UStartFinishGameScreen::PostNativeConstruct()
{
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->PickedButton = ContinueButton;
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
	}
}

void UStartFinishGameScreen::ContinueButtonOnClicked()
{
	if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if(const auto* const QuestManager = RedemptionGameModeBase->GetQuestManager(); IsValid(QuestManager)) {
			for (FQuestAndItsStage QuestAndItsStage : QuestManager->GetActiveOrFinishedQuestsAndTheirStages())
				if (QuestAndItsStage.QuestClass->GetDefaultObject<AQuest>()->GetQuestName().EqualTo(FText::FromString("Destroy the Undead")) &&
				QuestAndItsStage.QuestStage == QuestAndItsStage.QuestClass->GetDefaultObject<AQuest>()->GetFinalStage()) {
					UGameplayStatics::OpenLevel(GetWorld(), "Main Menu");
					break;
				}
			if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
				this->RemoveFromParent();
				UIManagerWorldSubsystem->StartFinishGameScreenWidget = nullptr;
				if (auto* const PlayerController = GetWorld()->GetFirstPlayerController<APlayerController>(); IsValid(PlayerController)) {
					PlayerController->bShowMouseCursor = false;
					PlayerController->bEnableClickEvents = false;
					PlayerController->bEnableMouseOverEvents = false;
					UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
					if (auto* const PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter()); IsValid(PlayerCharacter))
						PlayerCharacter->CanOpenOtherMenus = true;
				}
			}
		}
}

void UStartFinishGameScreen::OnWidgetShowLogic()
{
	if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (const auto* const QuestManager = RedemptionGameModeBase->GetQuestManager(); IsValid(QuestManager)) {
			bool FoundQuest = false;
			for (FQuestAndItsStage QuestAndItsStage : QuestManager->GetActiveOrFinishedQuestsAndTheirStages())
				if (QuestAndItsStage.QuestClass->GetDefaultObject<AQuest>()->GetQuestName().EqualTo(FText::FromString("Destroy the Undead")) &&
					QuestAndItsStage.QuestStage == QuestAndItsStage.QuestClass->GetDefaultObject<AQuest>()->GetFinalStage()) {
					MainTextBlock->SetText(FinishText);
					FoundQuest = true;
					break;
				}
			if (!FoundQuest)
				MainTextBlock->SetText(StartText);
			if (auto* const PlayerController = GetWorld()->GetFirstPlayerController<APlayerController>(); IsValid(PlayerController)) {
				PlayerController->bShowMouseCursor = true;
				PlayerController->bEnableClickEvents = true;
				PlayerController->bEnableMouseOverEvents = true;
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, this, EMouseLockMode::DoNotLock);
				if (auto* const PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter()); IsValid(PlayerCharacter))
					PlayerCharacter->CanOpenOtherMenus = false;
			}
		}
}
