// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTargetEntry.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Redemption\Dynamics\World\Items\UseItems\RestorationItem.h"
#include "Redemption/Miscellaneous/InventoryActions.h"
#include "Kismet/GameplayStatics.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Miscellaneous\InventoryScrollBoxEntryWidget.h"

bool UInventoryTargetEntry::Initialize()
{
    const bool bSuccess = Super::Initialize();
    if (IsValid(NameButtonWithHoverColors)) {
        NameButtonWithHoverColors->OnClicked.AddDynamic(this, &UInventoryTargetEntry::NameButtonOnClicked);
        NameButtonWithHoverColors->OnHovered.AddDynamic(this, &UInventoryTargetEntry::NameButtonOnHovered);
    }
    if (!bSuccess) return false;
    return bSuccess;
}

void UInventoryTargetEntry::NativeConstruct()
{
	Super::NativeConstruct();
    HealthProgressBar->PercentDelegate.BindUFunction(this, "GetHealth");
    HealthProgressBar->SynchronizeProperties();
    ManaProgressBar->PercentDelegate.BindUFunction(this, "GetMana");
    ManaProgressBar->SynchronizeProperties();
}

void UInventoryTargetEntry::NameButtonOnClicked()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter())) {
        //Find Item's widget in inventory
        if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
            if (UInventoryMenu* InventoryMenuWidget = UIManagerWorldSubsystem->InventoryMenuWidget; IsValid(InventoryMenuWidget) && IsValid(InventoryMenuWidget->GetPickedItem())) {
                URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
                UInventoryScrollBoxEntryWidget* EntryWidget = InventoryActions::FindItemInventoryEntryWidget(InventoryMenuWidget->GetPickedItem(), InventoryMenuWidget->GetInventoryScrollBox());
                if (ARestorationItem* RestorationItem = Cast<ARestorationItem>(InventoryMenuWidget->GetPickedItem()); IsValid(RestorationItem)) {
                    bool ItemHasBeenUsed = false;
                    if (IsValid(Player)) {
                        if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && Player->CurrentHP < Player->MaxHP) {
                            Player->CurrentHP += RestorationItem->GetRestorationValuePercent();
                            ItemHasBeenUsed = true;
                            if (Player->CurrentHP > Player->MaxHP)
                                Player->CurrentHP = Player->MaxHP;
                        }
                        else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && Player->CurrentMana < Player->MaxMana) {
                            Player->CurrentMana += RestorationItem->GetRestorationValuePercent();
                            ItemHasBeenUsed = true;
                            if (Player->CurrentMana > Player->MaxMana)
                                Player->CurrentMana = Player->MaxMana;
                        }
                        else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && Player->CurrentMana >= Player->MaxMana)
                            InventoryMenuWidget->ActivateNotification(FText::FromString("Your mana is already full!!!"));
                        else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && Player->CurrentHP >= Player->MaxHP)
                            InventoryMenuWidget->ActivateNotification(FText::FromString("Your health is already full!!!"));
                    }
                    else if (IsValid(Ally)) {
                        if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && Ally->CurrentHP < Ally->MaxHP) {
                            Ally->CurrentHP += RestorationItem->GetRestorationValuePercent();
                            ItemHasBeenUsed = true;
                            if (Ally->CurrentHP > Ally->MaxHP)
                                Ally->CurrentHP = Ally->MaxHP;
                        }
                        else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && Ally->CurrentMana < Ally->MaxMana) {
                            Ally->CurrentMana += RestorationItem->GetRestorationValuePercent();
                            ItemHasBeenUsed = true;
                            if (Ally->CurrentMana > Ally->MaxMana)
                                Ally->CurrentMana = Ally->MaxMana;
                        }
                        else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA && Ally->CurrentMana >= Ally->MaxMana)
                            InventoryMenuWidget->ActivateNotification(FText::FromString("Your mana is already full!!!"));
                        else if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::HEALTH && Ally->CurrentHP >= Ally->MaxHP)
                            InventoryMenuWidget->ActivateNotification(FText::FromString("Your health is already full!!!"));
                    }
                    if (ItemHasBeenUsed) {
                        InventoryActions::RemoveItemFromGameInstance(GameInstance, InventoryMenuWidget->GetPickedItem());
                        if (EntryWidget->AmountOfItems <= 1) {
                            InventoryMenuWidget->SetPickedItem(nullptr);
                            InventoryMenuWidget->HideTargetsMenu();
                        }
                        InventoryActions::ItemAmountInInventoryLogic(EntryWidget, InventoryMenuWidget->GetInventoryScrollBox(), InventoryMenuWidget->GetPickedItem());
                        if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
                            UGameplayStatics::PlaySound2D(GetWorld(), RedemptionGameModeBase->GetAudioManager()->GetUseHealOrBuffSoundCue());
                    }
                }
            }
    }
}

void UInventoryTargetEntry::NameButtonOnHovered()
{
    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
        if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
            if (UInventoryMenu* InventoryMenuWidget = UIManagerWorldSubsystem->InventoryMenuWidget; IsValid(InventoryMenuWidget)) {
                InventoryMenuWidget->GetInventoryButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
                InventoryMenuWidget->GetMeleeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
                InventoryMenuWidget->GetRangeButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
                InventoryMenuWidget->GetHeadButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
                InventoryMenuWidget->GetTorseButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
                InventoryMenuWidget->GetHandButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
                InventoryMenuWidget->GetLowerArmorButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 0));
                if (InventoryMenuWidget->GetInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
                    InventoryMenuWidget->GetInventoryButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                if (InventoryMenuWidget->GetMeleeInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
                    InventoryMenuWidget->GetMeleeButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                if (InventoryMenuWidget->GetRangeInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
                    InventoryMenuWidget->GetRangeButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                if (InventoryMenuWidget->GetHeadInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
                    InventoryMenuWidget->GetHeadButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                if (InventoryMenuWidget->GetTorseInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
                    InventoryMenuWidget->GetTorseButton()->SetBackgroundColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
                if (InventoryMenuWidget->GetHandInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
                    InventoryMenuWidget->GetHandButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                if (InventoryMenuWidget->GetLowerArmorInventoryBorder()->GetVisibility() == ESlateVisibility::Visible)
                    InventoryMenuWidget->GetLowerArmorButton()->SetBackgroundColor(FLinearColor(0, 1, 0, 1));
                if (IsValid(UIManagerWorldSubsystem->PickedButton)) {
                    if (UButtonWithHoverColors* ButtonWithColors = Cast<UButtonWithHoverColors>(UIManagerWorldSubsystem->PickedButton); IsValid(ButtonWithColors))
                        UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(ButtonWithColors->GetOnUnhoverColor());
                    else {
                        if (InventoryMenuWidget->GetInventoryScrollBox()->GetVisibility() == ESlateVisibility::Visible && InventoryMenuWidget->SelectedItemButtonIndex >= 0) {
                            if (InventoryMenuWidget->GetInventoryScrollBox()->GetAllChildren().Num() > InventoryMenuWidget->SelectedItemButtonIndex) {
                                if (InventoryMenuWidget->GetInventoryScrollBox()->GetChildAt(InventoryMenuWidget->SelectedItemButtonIndex) ==
                                    Cast<UWidget>(UIManagerWorldSubsystem->PickedButton->GetOuter()->GetOuter()))
                                    UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 1.f, 1.f));
                                else
                                    UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
                            }
                        }
                        else
                            UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
                    }
                }
                UIManagerWorldSubsystem->PickedButton = NameButtonWithHoverColors;
                NameButtonWithHoverColors->SetBackgroundColor(NameButtonWithHoverColors->GetOnHoverColor());
                for (uint8 Index = 0; Index < InventoryMenuWidget->GetTargetsVerticalBox()->GetAllChildren().Num(); Index++)
                    if (InventoryMenuWidget->GetTargetsVerticalBox()->GetChildAt(Index) == this) {
                        UIManagerWorldSubsystem->PickedButtonIndex = Index;
                        break;
                    }
                InventoryMenuWidget->SelectedPanelWidget = InventoryMenuWidget->GetTargetsVerticalBox();
                InventoryMenuWidget->IsSelectingTarget = true;
                InventoryMenuWidget->IsSelectingSpecificItem = false;
            }
}

float UInventoryTargetEntry::GetHealth() const
{
    if (IsValid(Ally)) {
        return Ally->CurrentHP / Ally->MaxHP;
    }
    else if(IsValid(Player)) {
        return Player->CurrentHP / Player->MaxHP;
    }
    return 0.f;
}

float UInventoryTargetEntry::GetMana() const
{
    if (IsValid(Ally)) {
        return Ally->CurrentMana / Ally->MaxMana;
    }
    else if (IsValid(Player)) {
        return Player->CurrentMana / Player->MaxMana;
    }
    return 0.f;
}

UButtonWithHoverColors* UInventoryTargetEntry::GetNameButtonWithHoverColors() const
{
    return NameButtonWithHoverColors;
}

void UInventoryTargetEntry::SetUpWidget(const ACombatAllyNPC* const AllyToSetUp)
{
    Ally = const_cast<ACombatAllyNPC*>(AllyToSetUp);
    NameTextBlock->SetText(FText::FromName(AllyToSetUp->GetCharacterName()));
}

void UInventoryTargetEntry::SetUpWidget(const APlayerCharacter* const PlayerToSetUp)
{
    Player = const_cast<APlayerCharacter*>(PlayerToSetUp);
    NameTextBlock->SetText(FText::FromString(PlayerToSetUp->CharacterName));
}
