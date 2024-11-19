// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "Redemption/GameInstance/RedemptionGameInstance.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UIManagerWorldSubsystem.h"
#include "..\UI\Menus\JournalMenu.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "Redemption/Characters/AI Controllers/NonCombat/TownNPCAIController.h"


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
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AQuestManager::LoadObjectFromGameInstance);
}

void AQuestManager::OnLoadMapQuestScripting()
{
	TArray<AActor*> CharactersInTheWorld{};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterInTheWorld::StaticClass(), CharactersInTheWorld);
	bool SpawnJolleen = false;
	for (uint16 Index = 0; Index < ActiveOrFinishedQuestsAndTheirStages.Num(); Index++) {
		if (ActiveOrFinishedQuestsAndTheirStages[Index].QuestClass->GetDefaultObject<AQuest>()->GetQuestName().EqualTo(FText::FromString("Rescue the Priest's daughter"))) {
			FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
			for (AActor* Actor : CharactersInTheWorld) {
				FName CharacterName{};
				if (ACharacterInTheWorld* CharacterInTheWorld = Cast<ACharacterInTheWorld>(Actor); IsValid(CharacterInTheWorld)) {
					CharacterName = CharacterInTheWorld->GetCharacterName();
					if (CharacterName.IsEqual(FName("Jolleen"))) {
						if (MapName == "Dungeon") {
							if (ActiveOrFinishedQuestsAndTheirStages[Index].QuestStage >= 4 && ActiveOrFinishedQuestsAndTheirStages[Index].QuestStage <= 6) {
								CharacterInTheWorld->SetSmartObject(nullptr);
								if (auto* const Character = Cast<ACharacter>(CharacterInTheWorld); IsValid(Character))
									if (auto* const TownNPCAIController = Character->GetController<ATownNPCAIController>(); IsValid(TownNPCAIController))
										TownNPCAIController->DisableMainBehavior();
								Actor->SetActorLocation(FVector(-280.0, 1910.0, 128.0));
								Actor->SetActorRotation(FRotator(0.0, -130.0, 0.0));
							}
							else if (ActiveOrFinishedQuestsAndTheirStages[Index].QuestStage == 7)
								Actor->Destroy();
						}
						else if (MapName == "Town") {
							if (ActiveOrFinishedQuestsAndTheirStages[Index].QuestStage == 7) {
								Actor->SetActorLocation(FVector(-30.0, -260.0, 538.0));
								Actor->SetActorRotation(FRotator(0.0, 200.0, 0.0));
								if (auto* const TownNPC = Cast<ATownNPC>(CharacterInTheWorld); IsValid(TownNPC))
									TownNPC->DialogueStage = 1;
							}
						}
						break;
					}
				}
			}
		}
	}
}

void AQuestManager::OnFinishQuestQuestScripting(const FText& QuestName)
{
	if (QuestName.EqualTo(FText::FromString("Destroy the Undead"))) {
		for (FQuestAndItsStage QuestAndItsStage : ActiveOrFinishedQuestsAndTheirStages)
			if (QuestAndItsStage.QuestClass->GetDefaultObject<AQuest>()->GetQuestName().EqualTo(FText::FromString("Destroy the Undead")) &&
				QuestAndItsStage.QuestStage == QuestAndItsStage.QuestClass->GetDefaultObject<AQuest>()->GetFinalStage()) {
				TArray<AActor*> EndGameActors{};
				UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), FName("EndGameActor"), EndGameActors);
				for (AActor* EndGameActor : EndGameActors) {
					EndGameActor->SetActorHiddenInGame(false);
				}
				break;
			}
	}
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
					if (QuestAndItsStage.QuestStage >= QuestDefaultObject->GetFinalStage())
						UIManagerWorldSubsystem->JournalMenuWidget->AddNewQuestEntry(QuestDefaultObject->GetQuestName(), true);
					else
						UIManagerWorldSubsystem->JournalMenuWidget->AddNewQuestEntry(QuestDefaultObject->GetQuestName(), false);
				}
			}
	}
	OnLoadMapQuestScripting();
}

void AQuestManager::ActivateNewQuest(const TSubclassOf<AQuest> NewQuestClass)
{
	if (IsValid(NewQuestClass)) {
		FQuestAndItsStage QuestAndItsStage{};
		QuestAndItsStage.QuestClass = NewQuestClass;
		ActiveOrFinishedQuestsAndTheirStages.Add(QuestAndItsStage);
		if(AQuest* const Quest = NewQuestClass->GetDefaultObject<AQuest>(); IsValid(Quest))
			if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
				if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {
					for(const auto JournalEntryAndItsStage : Quest->GetJournalEntriesAndTheirStages())
						if (JournalEntryAndItsStage.QuestStage == 0) {
							UIManagerWorldSubsystem->JournalMenuWidget->AddNewEntryToJournal(UIManagerWorldSubsystem->JournalMenuWidget->JournalEntryNextNumber,
							JournalEntryAndItsStage.JournalEntry, Quest->GetQuestName(), false);
							break;
						}
					UIManagerWorldSubsystem->JournalMenuWidget->AddNewQuestEntry(Quest->GetQuestName(), false);
				}
				if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
					FString NewQuestString{};
					NewQuestString = "New Quest: ";
					NewQuestString.Append(*Quest->GetQuestName().ToString());
					PlayerCharacter->CreateNotification(FText::FromString(NewQuestString), 2.5f);
				}
			}
	}
}

void AQuestManager::ActivateNewQuest(const TSubclassOf<AQuest> NewQuestClass, const int8 NewQuestStage)
{
	if (IsValid(NewQuestClass)) {
		FQuestAndItsStage QuestAndItsStage{};
		QuestAndItsStage.QuestClass = NewQuestClass;
		QuestAndItsStage.QuestStage = NewQuestStage;
		ActiveOrFinishedQuestsAndTheirStages.Add(QuestAndItsStage);
		if (AQuest* const Quest = NewQuestClass->GetDefaultObject<AQuest>(); IsValid(Quest))
			if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
				if (auto* const PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
					FString NewQuestString{};
					NewQuestString = "New Quest: ";
					NewQuestString.Append(*Quest->GetQuestName().ToString());
					PlayerCharacter->CreateNotification(FText::FromString(NewQuestString), 2.5f);
				}
				if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {
					for (const auto JournalEntryAndItsStage : Quest->GetJournalEntriesAndTheirStages())
						if (JournalEntryAndItsStage.QuestStage == NewQuestStage) {
							UIManagerWorldSubsystem->JournalMenuWidget->AddNewEntryToJournal(UIManagerWorldSubsystem->JournalMenuWidget->JournalEntryNextNumber,
								JournalEntryAndItsStage.JournalEntry, Quest->GetQuestName(), false);
							break;
						}
					UIManagerWorldSubsystem->JournalMenuWidget->AddNewQuestEntry(Quest->GetQuestName(), false);
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
				if (AQuest* const Quest = QuestToAdvance->GetDefaultObject<AQuest>(); IsValid(Quest))
					if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
						if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {
							if (Quest->GetJournalEntriesAndTheirStages().Num() > 0)
								UIManagerWorldSubsystem->JournalMenuWidget->AddNewEntryToJournal(UIManagerWorldSubsystem->JournalMenuWidget->JournalEntryNextNumber,
									Quest->GetJournalEntriesAndTheirStages()[QuestAndStage.QuestStage].JournalEntry, Quest->GetQuestName(), false);
						}
				break;
			}
	}
}

void AQuestManager::AdvanceQuest(const TSubclassOf<AQuest> QuestToAdvance, const int8 StageToSetTo)
{
	if (IsValid(QuestToAdvance)) {
		for (auto& QuestAndStage : ActiveOrFinishedQuestsAndTheirStages)
			if (QuestAndStage.QuestClass == QuestToAdvance) {
				QuestAndStage.QuestStage = StageToSetTo;
				if (AQuest* const Quest = QuestToAdvance->GetDefaultObject<AQuest>(); IsValid(Quest))
					if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
						if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {
							if (Quest->GetJournalEntriesAndTheirStages().Num() > 0)
								UIManagerWorldSubsystem->JournalMenuWidget->AddNewEntryToJournal(UIManagerWorldSubsystem->JournalMenuWidget->JournalEntryNextNumber,
									Quest->GetJournalEntriesAndTheirStages()[QuestAndStage.QuestStage].JournalEntry, Quest->GetQuestName(), false);
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
				QuestAndStage.QuestStage = QuestToFinish->GetDefaultObject<AQuest>()->GetFinalStage();
		//Get rewards and set journal quest's button color to yellow.
		if (const AQuest* const QuestObject = QuestToFinish->GetDefaultObject<AQuest>(); IsValid(QuestObject)) {
			if (auto* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
				FString QuestCompletedString{};
				QuestCompletedString = "Quest Completed: ";
				QuestCompletedString.Append(*QuestObject->GetQuestName().ToString());
				PlayerCharacter->CreateNotification(FText::FromString(QuestCompletedString), 1.5f);
				PlayerCharacter->Gold += QuestObject->GetGoldReward();
				uint16 ExperienceRewardForEachPartyMember = QuestObject->GetExperienceReward() / (PlayerCharacter->GetAllies().Num() + 1);
				if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
					if (const auto* const LevelingUpManager = RedemptionGameModeBase->GetLevelingUpManager(); IsValid(RedemptionGameModeBase->GetLevelingUpManager())) {
						PlayerCharacter->CurrentExperience += ExperienceRewardForEachPartyMember;
						bool LeveledUp = RedemptionGameModeBase->GetLevelingUpManager()->LevelUp(PlayerCharacter, nullptr, true);
						for (ACombatAllyNPC* const CombatAllyNPC : PlayerCharacter->GetAllies()) {
							CombatAllyNPC->CurrentExperience += ExperienceRewardForEachPartyMember;
							if (!LeveledUp) {
								LeveledUp = RedemptionGameModeBase->GetLevelingUpManager()->LevelUp(CombatAllyNPC, nullptr, true);
							}
						}
					}
				if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
					if (IsValid(UIManagerWorldSubsystem->JournalMenuWidget)) {
						if (QuestObject->GetJournalEntriesAndTheirStages().Num() > 0)
							UIManagerWorldSubsystem->JournalMenuWidget->AddNewEntryToJournal(UIManagerWorldSubsystem->JournalMenuWidget->JournalEntryNextNumber,
								QuestObject->GetJournalEntriesAndTheirStages()[QuestObject->GetFinalStage()].JournalEntry, QuestObject->GetQuestName(), false);
						for (TSubclassOf<AGameItem> ItemRewardClass : QuestObject->GetItemsRewardClasses()) {
							UIManagerWorldSubsystem->InventoryMenuWidget->PickUpItem(ItemRewardClass, 1);
							FString NewItemString{};
							QuestCompletedString = "New Item: ";
							QuestCompletedString.Append(*ItemRewardClass->GetDefaultObject<AGameItem>()->GetItemName().ToString());
							PlayerCharacter->CreateNotification(FText::FromString(QuestCompletedString), 1.5f);
						}
						for (UWidget* const Widget : UIManagerWorldSubsystem->JournalMenuWidget->GetQuestsScrollBox()->GetAllChildren())
							if (UJournalQuestEntry* const JournalQuestEntry = Cast<UJournalQuestEntry>(Widget); IsValid(JournalQuestEntry)) {
								if (JournalQuestEntry->GetQuestNameTextBlockText().EqualTo(QuestObject->GetQuestName())) {
									JournalQuestEntry->SetMainButtonBackgroundColor(FLinearColor(1.f, 1.f, 0.f, 1.f));
									JournalQuestEntry->FinishedQuest = true;
								}
							}
					}
				}
				OnFinishQuestQuestScripting(QuestObject->GetQuestName());
			}
		}
	}
}

const TArray<FQuestAndItsStage>& AQuestManager::GetActiveOrFinishedQuestsAndTheirStages() const
{
	return ActiveOrFinishedQuestsAndTheirStages;
}
