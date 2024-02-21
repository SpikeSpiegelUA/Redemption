// Fill out your copyright notice in the Description page of Project Settings.


#include "..\UI\Screens\BattleResultsScreen.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Components/StackBox.h"
#include "Components/TextBlock.h"
#include "..\UI\Miscellaneous\CharacterLevelingUp.h"
#include "Components/Button.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

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
	OnWidgetShowActions();
}

void UBattleResultsScreen::ContinueButtonOnClicked()
{
	this->RemoveFromParent();
	this->ConditionalBeginDestroy();
	if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		RedemptionGameModeBase->GetGameManager()->EndBattle();
	if(auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		UIManagerWorldSubsystem->BattleResultsScreenWidget = nullptr;
}

void UBattleResultsScreen::NextCharacterButtonOnClicked()
{

}

void UBattleResultsScreen::PreviousCharacterButtonOnClicked()
{

}

void UBattleResultsScreen::ContinueButtonOnHovered()
{

}

void UBattleResultsScreen::NextCharacterButtonOnHovered()
{

}

void UBattleResultsScreen::PreviousCharacterButtonOnHovered()
{

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
		PlayerCharacter->Gold += TotalGoldReward;
		if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
			SetCharacterInfo(RedemptionGameModeBase->GetBattleManager()->CombatPlayerCharacter);
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
		if (RedemptionGameModeBase->GetLevelingUpManager()->LevelUp(CombatAllyToSetInfo->Level, CombatAllyToSetInfo->CurrentExperience, CharacterLevelingUpWidget->GetNextLevelProgressBar())) {
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
					LevelingUpEntryWidget->AddToViewport();
				}
			}
		}
	}
}

void UBattleResultsScreen::SetGoldTextBlock(const FText& TextToSet)
{
	GoldTextBlock->SetText(TextToSet);
}

void UBattleResultsScreen::SetCharacterPortraitImage(const UTexture* const ImageToSet)
{
	CharacterPortraitImage->Brush.SetResourceObject(const_cast<UTexture*>(ImageToSet));
	CharacterPortraitImage->Brush.SetImageSize(FVector2D(120, 100));
}

UTextBlock* UBattleResultsScreen::GetGoldTextBlock() const
{
	return GoldTextBlock;
}

UButton* UBattleResultsScreen::GetContinueButton() const
{
	return ContinueButton;
}


