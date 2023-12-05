// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSlotEntry.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "..\Characters\Player\PlayerCharacter.h"

bool USaveSlotEntry::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(SlotButton)) {
		SlotButton->OnClicked.AddDynamic(this, &USaveSlotEntry::SlotButtonOnClicked);
		SlotButton->OnHovered.AddDynamic(this, &USaveSlotEntry::SlotButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void USaveSlotEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void USaveSlotEntry::SlotButtonOnClicked()
{
	if (URedemptionGameInstance* RedemptionGameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance()); IsValid(RedemptionGameInstance)) {
		FString SaveFileName = "Save " + FString::FromInt(SlotIndex);
		RedemptionGameInstance->SaveFileName = SaveFileName;
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
			if (IsValid(PlayerCharacter->GetSaveLoadGameMenuWidget())) {
				if (IsValid(PlayerCharacter->GetSaveLoadGameMenuWidget()->SelectedSaveSlotButton))
					PlayerCharacter->GetSaveLoadGameMenuWidget()->SelectedSaveSlotButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
				PlayerCharacter->GetSaveLoadGameMenuWidget()->SelectedSaveSlotButton = SlotButton;
				SlotButton->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
			}
	}
}

void USaveSlotEntry::SlotButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) 
		if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())){
			if (IsValid(UIManagerWorldSubsystem->PickedButton))
				if (UIManagerWorldSubsystem->PickedButton == PlayerCharacter->GetSaveLoadGameMenuWidget()->SelectedSaveSlotButton)
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
				else
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
			UIManagerWorldSubsystem->PickedButton = SlotButton;
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			for (uint16 Index = 0; Index < PlayerCharacter->GetSaveLoadGameMenuWidget()->GetSaveSlotsScrollBox()->GetAllChildren().Num(); Index++)
				if (PlayerCharacter->GetSaveLoadGameMenuWidget()->GetSaveSlotsScrollBox()->GetAllChildren()[Index] == this) {
					UIManagerWorldSubsystem->PickedButtonIndex = Index;
					break;
				}
		}
}

uint16 USaveSlotEntry::GetSlotIndex() const
{
	return SlotIndex;
}

UButton* USaveSlotEntry::GetSlotButton() const
{
	return SlotButton;
}

UTextBlock* USaveSlotEntry::GetNameTextBlock() const
{
	return NameTextBlock;
}

void USaveSlotEntry::SetSlotIndex(const uint16 NewSlotIndex)
{
	SlotIndex = NewSlotIndex;
}

