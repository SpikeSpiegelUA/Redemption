// Fill out your copyright notice in the Description page of Project Settings.


#include "..\UI\Screens\BattleResultsScreen.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Components/StackBox.h"
#include "Components/TextBlock.h"
#include "..\UI\Miscellaneous\CharacterLevelingUp.h"
#include "Components/Button.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/ScrollBoxSlot.h"

bool UBattleResultsScreen::Initialize()
{
	const bool bSuccess = Super::Initialize();
	if (IsValid(ContinueButton)) {
		ContinueButton->OnClicked.AddDynamic(this, &UBattleResultsScreen::ContinueButtonOnClicked);
		ContinueButton->OnHovered.AddDynamic(this, &UBattleResultsScreen::ContinueButtonOnHovered);

		if (IsValid(PreviousCharacterButton)) {
			PreviousCharacterButton->OnClicked.AddDynamic(this, &UBattleResultsScreen::PreviousCharacterButtonOnClicked);
			PreviousCharacterButton->OnHovered.AddDynamic(this, &UBattleResultsScreen::PreviousCharacterButtonOnHovered);
		}
		if (IsValid(NextCharacterButton)) {
			NextCharacterButton->OnClicked.AddDynamic(this, &UBattleResultsScreen::NextCharacterButtonOnClicked);
			NextCharacterButton->OnHovered.AddDynamic(this, &UBattleResultsScreen::NextCharacterButtonOnHovered);
		}
	}
		if (!bSuccess) return false;
		return bSuccess;
}

void UBattleResultsScreen::NativeConstruct()
{
	Super::NativeConstruct();
	OnWidgetShowActions();
}

void UBattleResultsScreen::ContinueButtonOnClicked()
{
	this->RemoveFromParent();
	this->ConditionalBeginDestroy();
	if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		RedemptionGameModeBase->GetGameManager()->EndBattle();
	if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->PickedButton = nullptr;
		UIManagerWorldSubsystem->BattleResultsScreenWidget = nullptr;
	}
}

void UBattleResultsScreen::NextCharacterButtonOnClicked()
{
	if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
		if (RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer.Num() > 1) {
			if (CurrentCharacterIndex + 1 < RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer.Num()) {
				SetCharacterInfo(Cast<ACombatAllies>(RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer[CurrentCharacterIndex + 1]));
				CurrentCharacterIndex += 1;
			}
			else {
				SetCharacterInfo(Cast<ACombatAllies>(RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer[0]));
				CurrentCharacterIndex = 0;
			}
		}
	}
}

void UBattleResultsScreen::PreviousCharacterButtonOnClicked()
{
	if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
		if (RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer.Num() > 1) {
			if (CurrentCharacterIndex - 1 >= 0) {
				SetCharacterInfo(Cast<ACombatAllies>(RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer[CurrentCharacterIndex - 1]));
				CurrentCharacterIndex -= 1;
			}
			else {
				SetCharacterInfo(Cast<ACombatAllies>(RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer[RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer.Num() - 1]));
				CurrentCharacterIndex = RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer.Num() - 1;
			}
		}
	}
}

void UBattleResultsScreen::ContinueButtonOnHovered()
{
	ButtonOnHoveredActions(ContinueButton, 0);
}

void UBattleResultsScreen::NextCharacterButtonOnHovered()
{
	ButtonOnHoveredActions(NextCharacterButton, 0);
}

void UBattleResultsScreen::PreviousCharacterButtonOnHovered()
{
	ButtonOnHoveredActions(PreviousCharacterButton, 0);
}

void UBattleResultsScreen::OnWidgetShowActions()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		uint16 TotalExperienceReward = 0;
		uint16 TotalGoldReward = 0;
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
			for (ACombatNPC* CombatNPC : RedemptionGameModeBase->GetBattleManager()->BattleEnemies)
				if (ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(CombatNPC); IsValid(CombatEnemyNPC)) {
					TotalGoldReward += CombatEnemyNPC->GetGoldReward();
					TotalExperienceReward += CombatEnemyNPC->GetExperienceReward();
				}
			for (ACombatNPC* CombatNPC : RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer)
				if (ACombatAllies* CombatAllies = Cast<ACombatAllies>(CombatNPC); IsValid(CombatAllies))
					CombatAllies->CurrentExperience += TotalExperienceReward / RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer.Num();
		}
		TotalGoldReward += FMath::RandRange(0, PlayerCharacter->GetStat(ECharacterStats::LUCK) * 10);
		PlayerCharacter->Gold += TotalGoldReward;
		if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
			for(uint8 Index = 0; Index < RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer.Num(); Index++)
				if (RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer[Index] == RedemptionGameModeBase->GetBattleManager()->CombatPlayerCharacter) {
					CurrentCharacterIndex = Index;
					SetCharacterInfo(Cast<ACombatAllies>(RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer[Index]));
					break;
				}
		}
		//Set Gold text.
		FString StringToSet = "Gold: ";
		StringToSet.AppendInt(PlayerCharacter->Gold);
		StringToSet.Append(", +");
		StringToSet.AppendInt(TotalGoldReward);
		GoldTextBlock->SetText(FText::FromString(StringToSet));
	}
}

void UBattleResultsScreen::SetCharacterInfo(ACombatAllies* const CombatAllyToSetInfo)
{
	SetCharacterPortraitImage(CombatAllyToSetInfo->GetCharacterPortrait());
	FString StringToSet{};
	//Name text.
	StringToSet = "Name: ";
	StringToSet.Append(*CombatAllyToSetInfo->GetCharacterName().ToString());
	CharacterLevelingUpWidget->SetCharacterNameTextBlockText(FText::FromString(StringToSet));
	//Level text.
	StringToSet = "Level: ";
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
		if (RedemptionGameModeBase->GetLevelingUpManager()->LevelUp(CombatAllyToSetInfo, CharacterLevelingUpWidget->GetNextLevelProgressBar())) {
			StringToSet.AppendInt(CombatAllyToSetInfo->Level);
			StringToSet.Append(", ");
			StringToSet.Append("level up!!! ");
		}
		else {
			StringToSet.AppendInt(CombatAllyToSetInfo->Level);
			StringToSet.Append(", ");
		}
	}
	StringToSet.Append("Next level progress: ");
	CharacterLevelingUpWidget->SetCharacterLevelTextBlockText(FText::FromString(StringToSet));
	if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
		TArray<ECharacterSkills> CharacterSkills{};
		if (CombatAllyToSetInfo->GetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpMelee))
			CharacterSkills.Add(ECharacterSkills::MELEE);
		if (CombatAllyToSetInfo->GetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpRange))
			CharacterSkills.Add(ECharacterSkills::RANGE);
		if (CombatAllyToSetInfo->GetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpAssaultSpells))
			CharacterSkills.Add(ECharacterSkills::ASSAULTSPELLS);
		if (CombatAllyToSetInfo->GetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpDebuffSpells))
			CharacterSkills.Add(ECharacterSkills::DEBUFFSPELLS);
		if (CombatAllyToSetInfo->GetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpRestorationSpells))
			CharacterSkills.Add(ECharacterSkills::RESTORATIONSPELLS);
		if (CombatAllyToSetInfo->GetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpBuffSpells))
			CharacterSkills.Add(ECharacterSkills::BUFFSPELLS);
		if (CombatAllyToSetInfo->GetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpDefend))
			CharacterSkills.Add(ECharacterSkills::DEFEND);
		if (CombatAllyToSetInfo->GetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpPersuasion))
			CharacterSkills.Add(ECharacterSkills::PERSUASION);
		CharacterLevelingUpWidget->ClearLevelingUpScrollBox();
		for (ECharacterSkills CharacterSkill : CharacterSkills) {
			uint8 TotalCharacterSkillsLevelUp = 0;
			while (CombatAllyToSetInfo->GetSkillsProgress(CharacterSkill) >= 100 && CombatAllyToSetInfo->GetSkill(CharacterSkill) < 100) {
				CombatAllyToSetInfo->AddSkillsProgress(CharacterSkill, -100);
				TotalCharacterSkillsLevelUp += 1;
				CombatAllyToSetInfo->AddSkills(CharacterSkill, 1);
			}
			if (RedemptionGameModeBase->GetBattleManager()->CombatPlayerCharacter->GetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpMelee)) {
				LevelingUpEntryWidget = CreateWidget<ULevelingUpEntry>(Cast<APlayerController>(GetWorld()->GetFirstPlayerController()), LevelingUpEntryClass);
				if (IsValid(LevelingUpEntryWidget)) {
					FText EnumDisplayValue{};
					UEnum::GetDisplayValueAsText(CharacterSkill, EnumDisplayValue);
					FString WidgetStringToSet = *EnumDisplayValue.ToString();
					WidgetStringToSet.Append(", ");
					WidgetStringToSet.AppendInt(CombatAllyToSetInfo->GetSkill(CharacterSkill));
					WidgetStringToSet.Append("/100, +");
					WidgetStringToSet.AppendInt(TotalCharacterSkillsLevelUp);
					WidgetStringToSet.Append(". Next level progress: ");
					LevelingUpEntryWidget->SetSkillLevelingUpTextBlockText(FText::FromString(WidgetStringToSet));
					LevelingUpEntryWidget->SetNextLevelProgressBarPercent(CombatAllyToSetInfo->GetSkillsProgress(CharacterSkill));
					CharacterLevelingUpWidget->AddLevelingUpWidgetToLevelingUpScrollBox(LevelingUpEntryWidget);
					if(auto* LevelingUpEntryWidgetScrollBoxSlot = UWidgetLayoutLibrary::SlotAsScrollBoxSlot(LevelingUpEntryWidget); IsValid(LevelingUpEntryWidgetScrollBoxSlot))
						LevelingUpEntryWidgetScrollBoxSlot->SetPadding(FMargin(0.f, 25.f, 0.f, 0.f));
				}
			}
		}
	}
}

void UBattleResultsScreen::ButtonOnHoveredActions(UButton* const HoveredButton, const int8 Index)
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		if (IsValid(UIManagerWorldSubsystem->PickedButton))
			UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
		UIManagerWorldSubsystem->PickedButton = HoveredButton;
		UIManagerWorldSubsystem->PickedButton->SetBackgroundColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
		UIManagerWorldSubsystem->PickedButtonIndex = Index;
	}
}

void UBattleResultsScreen::SetGoldTextBlock(const FText& TextToSet)
{
	GoldTextBlock->SetText(TextToSet);
}

void UBattleResultsScreen::SetCharacterPortraitImage(const UTexture* const ImageToSet)
{
	FSlateBrush NewSlateBrush{};
	NewSlateBrush.SetResourceObject(const_cast<UTexture*>(ImageToSet));
	NewSlateBrush.SetImageSize(FVector2D(120, 100));
	CharacterPortraitImage->SetBrush(NewSlateBrush);
}

UTextBlock* UBattleResultsScreen::GetGoldTextBlock() const
{
	return GoldTextBlock;
}

UButton* UBattleResultsScreen::GetContinueButton() const
{
	return ContinueButton;
}


