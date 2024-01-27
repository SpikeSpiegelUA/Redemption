// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveLoadGameMenu.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Kismet/KismetStringLibrary.h"
#include "..\SaveSystem\RedemptionSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "..\UI\Menus\MainMenu.h"
#include "..\Miscellaneous\RedemptionGameModeBase.h"
#include "..\SaveSystem\RedemptionSaveGameForSaveSlots.h"

bool USaveLoadGameMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(SaveLoadButtonWithNeighbors)) {
		SaveLoadButtonWithNeighbors->OnClicked.AddDynamic(this, &USaveLoadGameMenu::SaveLoadButtonWithNeighborsOnClicked);
		SaveLoadButtonWithNeighbors->OnHovered.AddDynamic(this, &USaveLoadGameMenu::SaveLoadButtonWithNeighborsOnHovered);
	}
	if (IsValid(BackButtonWithNeighbors)) {
		BackButtonWithNeighbors->OnClicked.AddDynamic(this, &USaveLoadGameMenu::BackButtonWithNeighborsOnClicked);
		BackButtonWithNeighbors->OnHovered.AddDynamic(this, &USaveLoadGameMenu::BackButtonWithNeighborsOnHovered);
	}
	if (IsValid(OverwriteButtonWithNeighbors)) {
		OverwriteButtonWithNeighbors->OnClicked.AddDynamic(this, &USaveLoadGameMenu::OverwriteButtonWithNeighborsOnClicked);
		OverwriteButtonWithNeighbors->OnHovered.AddDynamic(this, &USaveLoadGameMenu::OverwriteButtonWithNeighborsOnHovered);
	}
	if (IsValid(DeleteButtonWithNeighbors)) {
		DeleteButtonWithNeighbors->OnClicked.AddDynamic(this, &USaveLoadGameMenu::DeleteButtonWithNeighborsOnClicked);
		DeleteButtonWithNeighbors->OnHovered.AddDynamic(this, &USaveLoadGameMenu::DeleteButtonWithNeighborsOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void USaveLoadGameMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void USaveLoadGameMenu::SaveLoadButtonWithNeighborsOnClicked()
{
	if (SaveLoadTextBlock->GetText().ToString() == "Load") {
		if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance))
			RedemptionGameInstance->LoadSavedGameMap(RedemptionGameInstance->SaveFileName);
	} 
	else if (SaveLoadTextBlock->GetText().ToString() == "Save") {
		if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance))
			RedemptionGameInstance->SaveGameAndCreateSlot(RedemptionGameInstance->SaveFileIndex);
	}
	DeleteHasBeenClicked = false;
	OverwriteHasBeenClicked = false;
	NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
}

void USaveLoadGameMenu::BackButtonWithNeighborsOnClicked()
{
	FString MapName = GetWorld()->GetMapName();
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (MapName == "UEDPIE_0_MainMenu") {
			this->RemoveFromParent();
			PlayerCharacter->GetMainMenuWidget()->AddToViewport();
			if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetMainMenuWidget()->GetNewGameButton();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
		}
		else if (MapName == "UEDPIE_0_Dungeon" || MapName == "UEDPIE_0_Town") {
			this->RemoveFromParent();
			PlayerCharacter->GetPauseMenuWidget()->AddToViewport();
			if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
				if (IsValid(UIManagerWorldSubsystem->PickedButton))
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
				UIManagerWorldSubsystem->PickedButton = PlayerCharacter->GetPauseMenuWidget()->GetResumeButton();
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				UIManagerWorldSubsystem->PickedButtonIndex = 0;
			}
		}
		if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance))
			RedemptionGameInstance->SaveFileName = "";
		DeleteHasBeenClicked = false;
		OverwriteHasBeenClicked = false;
		NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USaveLoadGameMenu::DeleteButtonWithNeighborsOnClicked()
{
	if (DeleteHasBeenClicked) {
		if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance))
			if (UGameplayStatics::DoesSaveGameExist(RedemptionGameInstance->SaveFileName, 0)) {
				URedemptionSaveGame* RedemptionSaveGame = Cast<URedemptionSaveGame>(UGameplayStatics::LoadGameFromSlot(RedemptionGameInstance->SaveFileName, 0));
				if (IsValid(RedemptionSaveGame)) {
					//Delete save and corresponding save slot.
					if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
						uint16 SaveSlotIndex = RedemptionSaveGame->SaveIndex;
						for (int8 Index = PlayerCharacter->GetSaveLoadGameMenuWidget()->GetSaveSlotsScrollBox()->GetAllChildren().Num() - 1; Index >= 0; Index--)
							if (USaveSlotEntry* SaveSlotEntry = Cast<USaveSlotEntry>(PlayerCharacter->GetSaveLoadGameMenuWidget()->GetSaveSlotsScrollBox()->GetAllChildren()[Index]); 
								IsValid(SaveSlotEntry) && IsValid(PlayerCharacter->GetSaveLoadGameMenuWidget()->GetSaveSlotsScrollBox()->GetAllChildren()[Index]))
								if (SaveSlotEntry->GetSlotIndex() == SaveSlotIndex)
									PlayerCharacter->GetSaveLoadGameMenuWidget()->GetSaveSlotsScrollBox()->GetChildAt(Index)->RemoveFromParent();
					}
					UGameplayStatics::DeleteGameInSlot(RedemptionGameInstance->SaveFileName, 0);
					//Save system file save.
					if (auto* LoadedRedemptionSaveGameForSaveSlots = Cast<URedemptionSaveGameForSaveSlots>
						(UGameplayStatics::CreateSaveGameObject(URedemptionSaveGameForSaveSlots::StaticClass())); IsValid(LoadedRedemptionSaveGameForSaveSlots)) {
						if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
							for (UWidget* SaveSlot : PlayerCharacter->GetSaveLoadGameMenuWidget()->GetSaveSlotsScrollBox()->GetAllChildren())
								if (USaveSlotEntry* SaveSlotEntry = Cast<USaveSlotEntry>(SaveSlot); IsValid(SaveSlotEntry))
									LoadedRedemptionSaveGameForSaveSlots->SaveSlotsIndexes.Add(SaveSlotEntry->GetSlotIndex());
						LoadedRedemptionSaveGameForSaveSlots->SaveFileIndex = RedemptionGameInstance->SaveFileIndex;
						UGameplayStatics::SaveGameToSlot(LoadedRedemptionSaveGameForSaveSlots, "System save", 0);
					}
					NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
					DeleteHasBeenClicked = false;
				}
			}
	}
	else {
		NotificationBorder->SetVisibility(ESlateVisibility::Visible);
		DeleteHasBeenClicked = true;
		OverwriteHasBeenClicked = false;
	}
}

void USaveLoadGameMenu::OverwriteButtonWithNeighborsOnClicked()
{
	if (OverwriteHasBeenClicked) {
		if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance))
			if (UGameplayStatics::DoesSaveGameExist(RedemptionGameInstance->SaveFileName, 0))
				if (URedemptionSaveGame* TemporarySaveGame = Cast<URedemptionSaveGame>(UGameplayStatics::LoadGameFromSlot(RedemptionGameInstance->SaveFileName, 0)); IsValid(TemporarySaveGame)) {
					RedemptionGameInstance->SaveGame(TemporarySaveGame->SaveIndex, true);
					NotificationBorder->SetVisibility(ESlateVisibility::Hidden);
					OverwriteHasBeenClicked = false;
				}
	}
	else {
		DeleteHasBeenClicked = false;
		OverwriteHasBeenClicked = true;
		NotificationBorder->SetVisibility(ESlateVisibility::Visible);
	}
}

void USaveLoadGameMenu::SaveLoadButtonWithNeighborsOnHovered()
{
	OnHoveredButtonActions(SaveLoadButtonWithNeighbors, 1);
}

void USaveLoadGameMenu::BackButtonWithNeighborsOnHovered()
{
	OnHoveredButtonActions(BackButtonWithNeighbors, 3);
}

void USaveLoadGameMenu::DeleteButtonWithNeighborsOnHovered()
{
	OnHoveredButtonActions(DeleteButtonWithNeighbors, 2);
}

void USaveLoadGameMenu::OverwriteButtonWithNeighborsOnHovered()
{
	OnHoveredButtonActions(OverwriteButtonWithNeighbors, 0);
}

void USaveLoadGameMenu::OnHoveredButtonActions(UButton* const HoveredButton, const int8 Index)
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			if (UIManagerWorldSubsystem->PickedButton == SelectedSaveSlotButton)
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
			else
				UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		UIManagerWorldSubsystem->PickedButton = HoveredButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButtonIndex = Index;
	}
}

void USaveLoadGameMenu::ToSaveTransition()
{
	SaveLoadTextBlock->SetText(FText::FromString("Save"));
	OverwriteButtonWithNeighbors->SetVisibility(ESlateVisibility::Visible);
}

void USaveLoadGameMenu::ToLoadTransition()
{
	SaveLoadTextBlock->SetText(FText::FromString("Load"));
	OverwriteButtonWithNeighbors->SetVisibility(ESlateVisibility::Hidden);
}

UButtonWithNeighbors* USaveLoadGameMenu::GetSaveLoadButtonWithNeighbors() const
{
	return SaveLoadButtonWithNeighbors;
}

UButtonWithNeighbors* USaveLoadGameMenu::GetBackButtonWithNeighbors() const
{
	return BackButtonWithNeighbors;
}

UButtonWithNeighbors* USaveLoadGameMenu::GetDeleteButtonWithNeighbors() const
{
	return DeleteButtonWithNeighbors;
}

UButtonWithNeighbors* USaveLoadGameMenu::GetOverwriteButtonWithNeighbors() const
{
	return OverwriteButtonWithNeighbors;
}

UScrollBox* USaveLoadGameMenu::GetSaveSlotsScrollBox() const
{
	return SaveSlotsScrollBox;
}

UTextBlock* USaveLoadGameMenu::GetSaveLoadTextBlock() const
{
	return SaveLoadTextBlock;
}

UBorder* USaveLoadGameMenu::GetNotificationBorder() const
{
	return NotificationBorder;
}

