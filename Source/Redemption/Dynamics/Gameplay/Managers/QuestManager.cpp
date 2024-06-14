// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "Redemption/GameInstance/RedemptionGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UIManagerWorldSubsystem.h"
#include "..\UI\Menus\JournalMenu.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
	if (auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		RedemptionGameModeBase->SetQuestManager(this);
	LoadObjectFromGameInstance();
}

// Called every frame
void AQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AQuestManager::LoadObjectFromGameInstance()
{
	if (auto* const RedemptionGameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(RedemptionGameInstance)) {
		FMemoryReader MemReader(RedemptionGameInstance->QuestManagerByteData);
		FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
		Ar.ArIsSaveGame = true;
		// Convert binary array back into actor's variables
		Serialize(Ar);
	}
	//Fill Journal Menu's QuestsBorder with quests, cause I don't want to store names for quests in the JournalMenu, when I can just take them from here.
	if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		//Firstly, active quests.
		for (FQuestAndItsStage QuestAndItsStage : ActiveOrFinishedQuestsAndTheirStages)
			if (const AQuest* const QuestDefaultObject = QuestAndItsStage.QuestClass->GetDefaultObject<AQuest>(); IsValid(QuestDefaultObject)) {
				if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {
					if (QuestAndItsStage.QuestStage == QuestDefaultObject->GetFinalStage())
						UIManagerWorldSubsystem->JournalMenuWidget->AddNewQuestEntry(QuestDefaultObject->GetQuestName(), true);
					else
						UIManagerWorldSubsystem->JournalMenuWidget->AddNewQuestEntry(QuestDefaultObject->GetQuestName(), false);
				}
			}
	}
}

void AQuestManager::ActivateNewQuest(const TSubclassOf<AQuest> NewQuestClass)
{
	if (IsValid(NewQuestClass)) {
		FQuestAndItsStage QuestAndItsStage{};
		QuestAndItsStage.QuestClass = NewQuestClass;
		ActiveOrFinishedQuestsAndTheirStages.Add(QuestAndItsStage);
		if(const AQuest* const Quest = NewQuestClass->GetDefaultObject<AQuest>(); IsValid(Quest))
			if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
				if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {
					if (Quest->GetJournalEntries().Num() > 0)
						UIManagerWorldSubsystem->JournalMenuWidget->AddNewEntryToJournal(UIManagerWorldSubsystem->JournalMenuWidget->JournalEntryNextNumber,
							Quest->GetJournalEntries()[0], Quest->GetQuestName(), false);
					UIManagerWorldSubsystem->JournalMenuWidget->AddNewQuestEntry(Quest->GetQuestName(), false);
				}
				if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
					FString NewQuestString{};
					NewQuestString = "New Quest: ";
					NewQuestString.Append(*Quest->GetQuestName().ToString());
					PlayerCharacter->CreateNotification(FText::FromString(NewQuestString));
				}
			}
	}
}

void AQuestManager::AdvanceQuest(const TSubclassOf<AQuest> QuestToAdvance)
{
	if(IsValid(QuestToAdvance)) {
		for (auto& QuestAndStage : ActiveOrFinishedQuestsAndTheirStages)
			if (QuestAndStage.QuestClass == QuestToAdvance) {
				QuestAndStage.QuestStage++;
				UE_LOG(LogTemp, Warning, TEXT("The QuestStage value is: %d"), QuestAndStage.QuestStage);
				if (const AQuest* const Quest = QuestToAdvance->GetDefaultObject<AQuest>(); IsValid(Quest))
					if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
						if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {
							if (Quest->GetJournalEntries().Num() > 0)
								UIManagerWorldSubsystem->JournalMenuWidget->AddNewEntryToJournal(UIManagerWorldSubsystem->JournalMenuWidget->JournalEntryNextNumber,
									Quest->GetJournalEntries()[QuestAndStage.QuestStage], Quest->GetQuestName(), false);
						}
				break;
			}
	}
}

void AQuestManager::FinishQuest(const TSubclassOf<AQuest> QuestToFinish)
{
	if (IsValid(QuestToFinish)) {
		//Set quest stage to the final stage.
		for (auto& QuestAndStage : ActiveOrFinishedQuestsAndTheirStages)
			if (QuestAndStage.QuestClass == QuestToFinish)
				QuestAndStage.QuestStage++;
		//Get rewards and set journal quest's button color to yellow.
		if(const AQuest* const QuestObject = QuestToFinish->GetDefaultObject<AQuest>(); IsValid(QuestObject)) {
			if (auto* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
				PlayerCharacter->Gold += QuestObject->GetGoldReward();
				uint16 ExperienceRewardForEachPartyMember = QuestObject->GetExperienceReward() / (PlayerCharacter->GetAllies().Num() + 1);
				PlayerCharacter->CurrentExperience += ExperienceRewardForEachPartyMember;
				for (ACombatAllyNPC* const CombatAllyNPC : PlayerCharacter->GetAllies())
					CombatAllyNPC->CurrentExperience += ExperienceRewardForEachPartyMember;
				if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
					for (TSubclassOf<AGameItem> ItemRewardClass : QuestObject->GetItemsRewardClasses())
						UIManagerWorldSubsystem->InventoryMenuWidget->PickUpItem(ItemRewardClass);
					for(UWidget* const Widget : UIManagerWorldSubsystem->JournalMenuWidget->GetQuestsScrollBox()->GetAllChildren())
						if (UJournalQuestEntry* const JournalQuestEntry = Cast<UJournalQuestEntry>(Widget); IsValid(JournalQuestEntry)) {
							if (JournalQuestEntry->GetQuestNameTextBlockText().EqualTo(QuestObject->GetQuestName())) {
								JournalQuestEntry->SetMainButtonBackgroundColor(FLinearColor(1.f, 1.f, 0.f, 1.f));
								JournalQuestEntry->FinishedQuest = true;
							}
						}
					FString QuestCompletedString{};
					QuestCompletedString = "Quest Completed: ";
					QuestCompletedString.Append(*QuestObject->GetQuestName().ToString());
					PlayerCharacter->CreateNotification(FText::FromString(QuestCompletedString));
				}
			}
		}
	}
}

const TArray<FQuestAndItsStage>& AQuestManager::GetActiveOrFinishedQuestsAndTheirStages() const
{
	return ActiveOrFinishedQuestsAndTheirStages;
}
