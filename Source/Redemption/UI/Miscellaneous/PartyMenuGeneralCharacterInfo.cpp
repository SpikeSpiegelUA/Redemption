// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyMenuGeneralCharacterInfo.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

bool UPartyMenuGeneralCharacterInfo::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(CharacterNameButton)) {
		CharacterNameButton->OnClicked.AddDynamic(this, &UPartyMenuGeneralCharacterInfo::CharacterNameButtonOnClicked);
		CharacterNameButton->OnHovered.AddDynamic(this, &UPartyMenuGeneralCharacterInfo::CharacterNameButtonOnHovered);
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
	CharacterPortraitImage->Brush.SetResourceObject(const_cast<UTexture*>(AllyToSet->GetCharacterPortrait()));
	CharacterPortraitImage->Brush.SetImageSize(FVector2D(120, 100));
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
	this->Ally = const_cast<ACombatAllyNPC*>(AllyToSet);
}

void UPartyMenuGeneralCharacterInfo::SetCharacterInfo(const APlayerCharacter* const Player)
{
	CharacterPortraitImage->Brush.SetResourceObject(const_cast<UTexture*>(Player->CharacterPortrait));
	CharacterPortraitImage->Brush.SetImageSize(FVector2D(120, 100));
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
						UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
					UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->DetailedCharacterInfoMenuWidget->GetToggleInfoButton();
					UIManagerWorldSubsystem->PickedButtonIndex = 0;
					UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
				}
			}
}

void UPartyMenuGeneralCharacterInfo::CharacterNameButtonOnHovered()
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 1.f));
		UIManagerWorldSubsystem->PickedButton = CharacterNameButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
			if(IsValid(UIManagerWorldSubsystem->PartyMenuWidget))
				UIManagerWorldSubsystem->PartyMenuWidget->IsSelectingCharacter = true;
	}
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

void UPartyMenuGeneralCharacterInfo::SetCharacterPortraitImage(const UTexture* const ImageToSet)
{
	CharacterPortraitImage->Brush.SetResourceObject(const_cast<UTexture*>(ImageToSet));
	CharacterPortraitImage->Brush.SetImageSize(FVector2D(120, 100));
}

UButton* UPartyMenuGeneralCharacterInfo::GetCharacterNameButton() const
{
	return CharacterNameButton;
}
