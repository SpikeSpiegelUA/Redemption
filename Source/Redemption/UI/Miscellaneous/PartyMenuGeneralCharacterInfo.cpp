// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyMenuGeneralCharacterInfo.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components\CanvasPanelSlot.h"
#include "Redemption/UI/Menus/PerksLevelingUpMenu.h"

bool UPartyMenuGeneralCharacterInfo::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(CharacterNameButton)) {
		CharacterNameButton->OnClicked.AddDynamic(this, &UPartyMenuGeneralCharacterInfo::CharacterNameButtonOnClicked);
		CharacterNameButton->OnHovered.AddDynamic(this, &UPartyMenuGeneralCharacterInfo::CharacterNameButtonOnHovered);
	}
	if (IsValid(PerksLevelingUpButton)) {
		PerksLevelingUpButton->OnClicked.AddDynamic(this, &UPartyMenuGeneralCharacterInfo::PerksLevelingUpButtonOnClicked);
		PerksLevelingUpButton->OnHovered.AddDynamic(this, &UPartyMenuGeneralCharacterInfo::PerksLevelingUpButtonOnHovered);
	}
	if (!bSuccess) return false;
	return bSuccess;
}

void UPartyMenuGeneralCharacterInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPartyMenuGeneralCharacterInfo::SetCharacterInfo(const ACombatAllyNPC* const AllyToSet)
{
	FSlateBrush NewSlateBrush{};
	NewSlateBrush.SetResourceObject(const_cast<UTexture*>(AllyToSet->GetCharacterPortrait()));
	NewSlateBrush.SetImageSize(FVector2D(120, 100));
	CharacterPortraitImage->SetBrush(NewSlateBrush);
	CharacterNameTextBlock->SetText(FText::FromName(AllyToSet->GetCharacterName()));
	FString StringToSet{};
	StringToSet.Append("Mana:");
	StringToSet.AppendInt(AllyToSet->CurrentMana);
	StringToSet.AppendChar('/');
	StringToSet.AppendInt(AllyToSet->MaxMana);
	CharacterManaTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("HP:");
	StringToSet.AppendInt(AllyToSet->CurrentHP);
	StringToSet.AppendChar('/');
	StringToSet.AppendInt(AllyToSet->MaxHP);
	CharacterHPTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Level: ");
	StringToSet.AppendInt(AllyToSet->Level);
	CharacterLevelTextBlock->SetText(FText::FromString(StringToSet));
	if (AllyToSet->LevelingUpCounter > 0) {
		FString LevelingUpString = "Level up!!!";
		PerksLevelingUpTextBlock->SetText(FText::FromString(LevelingUpString));
	}
	this->Ally = const_cast<ACombatAllyNPC*>(AllyToSet);
	PerksLevelingUpTextBlockLogic();
}

void UPartyMenuGeneralCharacterInfo::SetCharacterInfo(const APlayerCharacter* const Player)
{
	FSlateBrush NewSlateBrush{};
	NewSlateBrush.SetResourceObject(const_cast<UTexture*>(Player->CharacterPortrait));
	NewSlateBrush.SetImageSize(FVector2D(120, 100));
	CharacterPortraitImage->SetBrush(NewSlateBrush);
	CharacterNameTextBlock->SetText(FText::FromString(Player->CharacterName));
	FString StringToSet{};
	StringToSet.Append("Mana:");
	StringToSet.AppendInt(Player->CurrentMana);
	StringToSet.AppendChar('/');
	StringToSet.AppendInt(Player->MaxMana);
	CharacterManaTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("HP:");
	StringToSet.AppendInt(Player->CurrentHP);
	StringToSet.AppendChar('/');
	StringToSet.AppendInt(Player->MaxHP);
	CharacterHPTextBlock->SetText(FText::FromString(StringToSet));
	StringToSet.Empty();
	StringToSet.Append("Level: ");
	StringToSet.AppendInt(Player->Level);
	CharacterLevelTextBlock->SetText(FText::FromString(StringToSet));
	this->Ally = nullptr;
	PerksLevelingUpTextBlockLogic();
}

void UPartyMenuGeneralCharacterInfo::PerksLevelingUpTextBlockLogic()
{
	if (IsValid(Ally)) {
		if (Ally->LevelingUpCounter > 0) {
			FString LevelingUpString = "Level up!!!";
			PerksLevelingUpTextBlock->SetText(FText::FromString(LevelingUpString));
		}
		else {
			FString PerksString = "Perks";
			PerksLevelingUpTextBlock->SetText(FText::FromString(PerksString));
		}
	}
	else if (const auto* const Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(Player)) {
		if (Player->LevelingUpCounter > 0) {
			FString LevelingUpString = "Level up!!!";
			PerksLevelingUpTextBlock->SetText(FText::FromString(LevelingUpString));
		}
		else {
			FString PerksString = "Perks";
			PerksLevelingUpTextBlock->SetText(FText::FromString(PerksString));
		}
	}
}

void UPartyMenuGeneralCharacterInfo::CharacterNameButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
				if (IsValid(RedemptionGameModeBase->GetDetailedCharacterInfoMenuClass()))
					if (APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()); IsValid(PlayerController))
						UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget = CreateWidget<UDetailedCharacterInfoMenu>(
						PlayerController, RedemptionGameModeBase->GetDetailedCharacterInfoMenuClass());
				if (IsValid(UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget)) {
					UIManagerWorldSubsystem->PartyMenuWidget->RemoveFromParent();
					UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->AddToViewport();
					if (Ally != nullptr) {
						UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->Ally = Ally;
						UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->SetCharacterInfoForAlly();
					}
					else {
						UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->Ally = nullptr;
						UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->SetCharacterInfoForPlayer(PlayerCharacter);
					}
					if (IsValid(UIManagerWorldSubsystem->PickedButton))
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.5f));
					UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->GetToggleInfoButton();
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
			}
}

void UPartyMenuGeneralCharacterInfo::PerksLevelingUpButtonOnClicked()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
				if (APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()); IsValid(PlayerController)) {
					UIManagerWorldSubsystem->PerksLevelingUpMenuWidget = CreateWidget<UPerksLevelingUpMenu>(PlayerController, RedemptionGameModeBase->GetPerksLevelingUpMenuClass());
					UIManagerWorldSubsystem->CharacterPerksMenuWidget = CreateWidget<UCharacterPerks>(PlayerController, RedemptionGameModeBase->GetStandardCharacterPerksMenuClass());
					UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->GetMainCanvasPanel()->AddChildToCanvas(UIManagerWorldSubsystem->CharacterPerksMenuWidget);
					if (UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(UIManagerWorldSubsystem->CharacterPerksMenuWidget); IsValid(CanvasPanelSlot)) {
						FAnchors Anchors{};
						Anchors.Maximum = FVector2D(1.0, 1.0);
						CanvasPanelSlot->SetAnchors(Anchors);
						FMargin Offsets{};
						Offsets.Right = 1000.f;
						Offsets.Bottom = 130.f;
						CanvasPanelSlot->SetOffsets(Offsets);
					}
				}
				if (IsValid(UIManagerWorldSubsystem->PartyMenuWidget))
					UIManagerWorldSubsystem->PartyMenuWidget->RemoveFromParent();
				if (IsValid(UIManagerWorldSubsystem->CharacterPerksMenuWidget)) {
					UIManagerWorldSubsystem->CharacterPerksMenuWidget->SetCurrentlySelectedCategoryIndex(0);
					if(IsValid(Ally))
						UIManagerWorldSubsystem->CharacterPerksMenuWidget->SetPerks(Ally);
					else
						UIManagerWorldSubsystem->CharacterPerksMenuWidget->SetPerks(PlayerCharacter);
				}
				PerksLevelingUpButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
				//Stats leveling up logic.
				if (PerksLevelingUpTextBlock->GetText().ToString().Equals("Level up!!!")) 
					UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->StatsLevelingUpLogic();
				if (IsValid(UIManagerWorldSubsystem->PerksLevelingUpMenuWidget))
					UIManagerWorldSubsystem->PerksLevelingUpMenuWidget->AddToViewport();
			}
		}
}

void UPartyMenuGeneralCharacterInfo::ButtonOnHoveredActions(UButton* const HoveredButton, const int8 Index)
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.f));
		UIManagerWorldSubsystem->PickedButton = HoveredButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButtonIndex = Index;
		if (IsValid(UIManagerWorldSubsystem->PartyMenuWidget)) {
			for (uint8 LoopIndex = 0; Index < UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetAllChildren().Num(); LoopIndex++)
				if (const auto* const PartyMenuGeneralCharacterInfo = Cast<UPartyMenuGeneralCharacterInfo>(
					UIManagerWorldSubsystem->PartyMenuWidget->GetCharactersHorizontalBox()->GetChildAt(LoopIndex)); IsValid(PartyMenuGeneralCharacterInfo)) {
					if (PartyMenuGeneralCharacterInfo->GetPerksLevelingUpButton() == UIManagerWorldSubsystem->PickedButton ||
						PartyMenuGeneralCharacterInfo->GetCharacterNameButton() == UIManagerWorldSubsystem->PickedButton) {
						UIManagerWorldSubsystem->PartyMenuWidget->SelectedGeneralCharacterInfoIndex = LoopIndex;
						break;
					}
				}
			UIManagerWorldSubsystem->PartyMenuWidget->IsSelectingCharacter = true;
		}
	}
}

void UPartyMenuGeneralCharacterInfo::PerksLevelingUpButtonOnHovered()
{
	ButtonOnHoveredActions(PerksLevelingUpButton, 1);
}

void UPartyMenuGeneralCharacterInfo::CharacterNameButtonOnHovered()
{
	ButtonOnHoveredActions(CharacterNameButton, 0);
}

void UPartyMenuGeneralCharacterInfo::SetCharacterNameTextBlockText(const FText& TextToSet)
{
	CharacterNameTextBlock->SetText(TextToSet);
}

void UPartyMenuGeneralCharacterInfo::SetCharacterLevelTextBlockText(const FText& TextToSet)
{
	CharacterLevelTextBlock->SetText(TextToSet);
}

void UPartyMenuGeneralCharacterInfo::SetCharacterHPTextBlockText(const FText& TextToSet)
{
	CharacterHPTextBlock->SetText(TextToSet);
}

void UPartyMenuGeneralCharacterInfo::SetCharacterManaTextBlockText(const FText& TextToSet)
{
	CharacterManaTextBlock->SetText(TextToSet);
}

void UPartyMenuGeneralCharacterInfo::SetPerksLevelingUpTextBlockText(const FText& TextToSet)
{
	PerksLevelingUpTextBlock->SetText(TextToSet);
}

void UPartyMenuGeneralCharacterInfo::SetCharacterPortraitImage(const UTexture* const ImageToSet)
{
	FSlateBrush NewSlateBrush{};
	NewSlateBrush.SetResourceObject(const_cast<UTexture*>(ImageToSet));
	NewSlateBrush.SetImageSize(FVector2D(120, 100));
	CharacterPortraitImage->SetBrush(NewSlateBrush);
}

UButton* UPartyMenuGeneralCharacterInfo::GetCharacterNameButton() const
{
	return CharacterNameButton;
}

UButton* UPartyMenuGeneralCharacterInfo::GetPerksLevelingUpButton() const
{
	return PerksLevelingUpButton;
}
