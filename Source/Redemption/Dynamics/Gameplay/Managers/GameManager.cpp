// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "..\UI\Menus\BattleMenu.h"
#include "Kismet/GameplayStatics.h"
#include "..\UI\Screens\ToBattleTransitionScreen.h"
#include "..\Characters\NonCombat\NonCombatEnemyNPC.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "UIManagerWorldSubsystem.h"
#include "..\Characters\Combat\CombatAllyNPC.h"
#include "Components/StackBox.h"
#include "..\GameInstance\RedemptionGameInstance.h"
#include "..\Miscellaneous\ArrayActions.h"
#include "..\Characters\AI Controllers\Combat\CombatEnemyNPCAIController.h"
#include "..\Miscellaneous\ArrayActions.h"
#include "..\Characters\AI Controllers\Combat\CombatAllyNPCAIController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "QuestManager.h"
#include <Redemption/Characters/NonCombat/TownNPCWithBattleEnemies.h>

// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		RedemptionGameModeBase->SetGameManager(this);
	FString MapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (MapName == "Dungeon") 
		EnemySpawn();
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Set up battle stage
void AGameManager::StartBattle(AActor* const AttackingNPC, const EBattleSide& FirstTurnBattleSide)
{
	if (!ToBattleTransitionScreen->IsInViewport()) {
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
			if (ABattleManager* BattleManager = RedemptionGameModeBase->GetBattleManager(); IsValid(PlayerCharacter) && IsValid(BattleManager)) {
				PlayerLastLocation = PlayerCharacter->GetActorLocation();
				//Set player's last rotation and location, teleport player to battle scene, destroy roaming NPC, disable input and battle UI widgets
				if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()); IsValid(PC))
					if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
						if (IsValid(RedemptionGameModeBase->GetBattleMenuClass()))
							UIManagerWorldSubsystem->BattleMenuWidget = CreateWidget<UBattleMenu>(Cast<APlayerController>(PlayerCharacter->GetController())
								, RedemptionGameModeBase->GetBattleMenuClass());
						if (IsValid(RedemptionGameModeBase->GetSpellBattleMenuClass()))
							UIManagerWorldSubsystem->SpellBattleMenuWidget = CreateWidget<USpellBattleMenu>(Cast<APlayerController>(PlayerCharacter->GetController()), RedemptionGameModeBase->GetSpellBattleMenuClass());
						if (IsValid(RedemptionGameModeBase->GetSkillBattleMenuClass()))
							UIManagerWorldSubsystem->SkillBattleMenuWidget = CreateWidget<USkillBattleMenu>(Cast<APlayerController>(PlayerCharacter->GetController()), RedemptionGameModeBase->GetSkillBattleMenuClass());
						ToBattleTransitionScreen->AddToViewport(0);
						if (FirstTurnBattleSide == EBattleSide::ALLIES)
							ToBattleTransitionScreen->SetMainTextBlockText(FText::FromString("Initiative!"));
						else
							ToBattleTransitionScreen->SetMainTextBlockText(FText::FromString("Ambush!"));
						FTimerDelegate ToBattleTransitionTimerDelegate = FTimerDelegate::CreateUObject(this, &AGameManager::ToBattleTransition, FirstTurnBattleSide);
						FTimerHandle ToBattleTransitionTimerHandle{};
						GetWorld()->GetTimerManager().SetTimer(ToBattleTransitionTimerHandle, ToBattleTransitionTimerDelegate, 2.f, false);
						//Create enemies and add them to BattleEnemies array in battle manager
						TArray<TSubclassOf<ACombatEnemyNPC>> BattleEnemies{};
						if (ANonCombatEnemyNPC* NonCombatEnemyNPC = Cast<ANonCombatEnemyNPC>(AttackingNPC); IsValid(NonCombatEnemyNPC))
							BattleEnemies = NonCombatEnemyNPC->GetBattleEnemies();
						else if (ATownNPCWithBattleEnemies* TownNPCWithBattleEnemies = Cast<ATownNPCWithBattleEnemies>(AttackingNPC); IsValid(TownNPCWithBattleEnemies))
							BattleEnemies = TownNPCWithBattleEnemies->GetBattleEnemies();
						ArrayActions::ShuffleArray(BattleEnemies);
						for (uint8 i = 0; i < BattleEnemies.Num() && i < 4; i++) {
							const FVector Location = EnemyBattleSpawns[i]->GetActorLocation();
							ACombatEnemyNPC* SpawnedEnemy = GetWorld()->SpawnActor<ACombatEnemyNPC>(BattleEnemies[i], Location, EnemyBattleSpawns[i]->GetActorRotation());
							ASmartObject* AISmartObject = GetWorld()->SpawnActor<ASmartObject>(SpawnedEnemy->GetAIClass());
							if (IsValid(SpawnedEnemy) && IsValid(AISmartObject))
								SpawnedEnemy->SetSmartObject(AISmartObject);
							SpawnedEnemy->SetStartLocation(EnemyBattleSpawns[i]);
							SpawnedEnemy->SetStartRotation(SpawnedEnemy->GetActorRotation());
							if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(SpawnedEnemy->GetController()); IsValid(AIController))
								AIController->SetDynamicSubtree();
							BattleManager->BattleEnemies.Add(SpawnedEnemy);
						}
						//Shuffle Allies and create combat player character
						//But firstly enable gravity for Allies. Yeah, I spent quite a lot of time on this, idiot.
						TArray<ACombatAllies*> ShuffledCombatAllies{};
						ACombatPlayerCharacter* CombatPlayerCharacter = GetWorld()->SpawnActor<ACombatPlayerCharacter>(CombatPlayerCharacterClass);
						CombatPlayerCharacter->SetActorLocation(AlliesPlayerBattleSpawns[3]->GetActorLocation());
						CombatPlayerCharacter->SetActorRotation(FRotator(0, 180, 0));
						CombatPlayerCharacter->SetStartLocation(AlliesPlayerBattleSpawns[3]);
						CombatPlayerCharacter->SetStartRotation(CombatPlayerCharacter->GetActorRotation());
						for (const TSubclassOf<APerk> PerkClass : CombatPlayerCharacter->ActivatedPerks)
							CombatPlayerCharacter->Effects.Add(CombatPlayerCharacter->ConvertActivatedPerkToEffect(PerkClass->GetDefaultObject<APerk>()));
						BattleManager->BattleAlliesPlayer.Add(CombatPlayerCharacter);
						BattleManager->CombatPlayerCharacter = CombatPlayerCharacter;
						for (ACombatAllyNPC* CombatAllyNPC : PlayerCharacter->GetAllies())
							ShuffledCombatAllies.Add(CombatAllyNPC);
						ArrayActions::ShuffleArray(ShuffledCombatAllies);
						PlayerCharacter->GetAllies().Empty();
						for (ACombatAllies* CombatAllies : ShuffledCombatAllies)
							PlayerCharacter->GetAllies().Add(Cast<ACombatAllyNPC>(CombatAllies));
						for (uint8 i = 0; i < PlayerCharacter->GetAllies().Num() && i < 4; i++) {
							if (IsValid(PlayerCharacter->GetAllies()[i])) {
								const FVector Location = AlliesPlayerBattleSpawns[i]->GetActorLocation();
								PlayerCharacter->GetAllies()[i]->SetActorLocation(Location);
								PlayerCharacter->GetAllies()[i]->SetActorRotation(FRotator(0.0, 180.0, 0.0));
								PlayerCharacter->GetAllies()[i]->SetStartLocation(AlliesPlayerBattleSpawns[i]);
								PlayerCharacter->GetAllies()[i]->SetStartRotation(PlayerCharacter->GetAllies()[i]->GetActorRotation());
								for (const TSubclassOf<APerk> PerkClass : PlayerCharacter->GetAllies()[i]->ActivatedPerks)
									PlayerCharacter->GetAllies()[i]->Effects.Add(PlayerCharacter->GetAllies()[i]->ConvertActivatedPerkToEffect(PerkClass->GetDefaultObject<APerk>()));
								if (IsValid(PlayerCharacter->GetAllies()[i]->GetAIClass()))
									if (ASmartObject* AISmartObject = GetWorld()->SpawnActor<ASmartObject>(PlayerCharacter->GetAllies()[i]->GetAIClass()); IsValid(AISmartObject))
										PlayerCharacter->GetAllies()[i]->SetSmartObject(AISmartObject);
								ACombatAllyNPCAIController* AIController = Cast<ACombatAllyNPCAIController>(PlayerCharacter->GetAllies()[i]->GetController());
								if (IsValid(AIController))
									AIController->SetDynamicSubtree();
								if (IsValid(BattleManager)) {
									BattleManager->BattleAlliesPlayer.Add(PlayerCharacter->GetAllies()[i]);
								}
							}
						}
						//Player Character Index.
						UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->PercentDelegate.Clear();
						UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->PercentDelegate.Clear();
						UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesInfoVerticalBoxes()[0]->SetVisibility(ESlateVisibility::Visible);
						UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->PercentDelegate.BindUFunction(CombatPlayerCharacter, "GetHealthPercentage");
						UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->PercentDelegate.BindUFunction(CombatPlayerCharacter, "GetManaPercentage");
						UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->SynchronizeProperties();
						UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->SynchronizeProperties();
						UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesNameTextBlockes()[0]->SetText(FText::FromName(CombatPlayerCharacter->GetCharacterName()));
						//Set camera position and set non combat player position to PlayerPlane's position
						BattleManager->CurrentTurnCombatNPCIndex = BattleManager->BattleAlliesPlayer.Num() - 1;
						BattleManager->SetBehindPlayerCameraLocation(Cast<ACombatStartLocation>(BattleManager->BattleAlliesPlayer
							[BattleManager->BattleAlliesPlayer.Num() - 1]->GetStartLocation())->CombatCameraLocation);
						PlayerCharacter->SetCanInput(false);
						PlayerCharacter->SetActorLocation(FVector(1350, 5610, -960));
						//PC->ActivateTouchInterface(PlayerCharacter->GetEmptyTouchInterface());
						FViewTargetTransitionParams Params;
						PC->SetViewTarget(RedemptionGameModeBase->GetBattleManager()->GetBehindPlayerCamera(), Params);
						UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC, UIManagerWorldSubsystem->BattleMenuWidget, EMouseLockMode::DoNotLock);
						//Background Music set
						RedemptionGameModeBase->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponents[RedemptionGameModeBase->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
						URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
						if (IsValid(GameInstance)) {
							bool SelectingMusic = true;
							do {
								int8 RandomBGMusicIndex = FMath::RandRange(0, RedemptionGameModeBase->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents.Num() - 1);
								if (RandomBGMusicIndex != GameInstance->InstancePreviousCombatBGMusicIndex) {
									RedemptionGameModeBase->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[RandomBGMusicIndex]->Play();
									RedemptionGameModeBase->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[RandomBGMusicIndex]->SetPaused(false);
									RedemptionGameModeBase->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic = RandomBGMusicIndex;
									GameInstance->InstancePreviousCombatBGMusicIndex = RandomBGMusicIndex;
									SelectingMusic = false;
								}
							} while (SelectingMusic);
						}
						AttackingCharacterNPC = Cast<ACharacterInTheWorld>(AttackingNPC);
					}
			}
	}
}

void AGameManager::ToBattleTransition(const EBattleSide FirstTurnBattleSide)
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (ABattleManager* BattleManager = RedemptionGameModeBase->GetBattleManager(); IsValid(PlayerCharacter) && IsValid(BattleManager)) {
			if (FirstTurnBattleSide == EBattleSide::ALLIES) {
				for (uint8 Index = 0; Index < BattleManager->BattleAlliesPlayer.Num(); Index++)
					BattleManager->AlliesPlayerTurnQueue.Add(Index);
				ArrayActions::ShuffleArray<uint8>(BattleManager->AlliesPlayerTurnQueue);
				BattleManager->CurrentTurnCombatNPCIndex = BattleManager->AlliesPlayerTurnQueue[0];
				BattleManager->SetBehindPlayerCameraLocation(Cast<ACombatStartLocation>(BattleManager->BattleAlliesPlayer
					[BattleManager->AlliesPlayerTurnQueue[0]]->GetStartLocation())->CombatCameraLocation);
				if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
					if (IsValid(UIManagerWorldSubsystem->BattleMenuWidget)) {
						if (IsValid(UIManagerWorldSubsystem->BattleMenuWidget)) {
							UIManagerWorldSubsystem->BattleMenuWidget->AddToViewport();
							UIManagerWorldSubsystem->BattleMenuWidget->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
							UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingAction = true;
						}
						UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->BattleMenuWidget->GetAttackButton();
						UIManagerWorldSubsystem->PickedButtonIndex = 0;
					}
					UIManagerWorldSubsystem->SkillBattleMenuWidget->ResetSkillsScrollBox();
					for (TSubclassOf<ASpell> SpellClass : BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnCombatNPCIndex]->GetAvailableSpells())
						UIManagerWorldSubsystem->SkillBattleMenuWidget->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
					if (UAlliesInfoBars* AlliesInfoBarsWidget = UIManagerWorldSubsystem->AlliesInfoBarsWidget; IsValid(AlliesInfoBarsWidget)) {
						if (AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder < AlliesInfoBarsWidget->GetAlliesNameBorders().Num())
							AlliesInfoBarsWidget->GetAlliesNameBorders()[AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder]->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
						AlliesInfoBarsWidget->GetAlliesNameBorders()[BattleManager->CurrentTurnCombatNPCIndex]->SetBrushColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
						AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder = BattleManager->CurrentTurnCombatNPCIndex;
					}
					if (BattleManager->BattleAlliesPlayer[BattleManager->AlliesPlayerTurnQueue[0]] == BattleManager->CombatPlayerCharacter) {
						UIManagerWorldSubsystem->BattleMenuWidget->GetItemButton()->SetVisibility(ESlateVisibility::Visible);
						UIManagerWorldSubsystem->BattleMenuWidget->GetTalkButton()->SetVisibility(ESlateVisibility::Visible);
						UIManagerWorldSubsystem->BattleMenuWidget->GetSpellButton()->SetVisibility(ESlateVisibility::Visible);
					}
					else {
						UIManagerWorldSubsystem->BattleMenuWidget->GetItemButton()->SetVisibility(ESlateVisibility::Collapsed);
						UIManagerWorldSubsystem->BattleMenuWidget->GetTalkButton()->SetVisibility(ESlateVisibility::Collapsed);
						UIManagerWorldSubsystem->BattleMenuWidget->GetSpellButton()->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
				BattleManager->AlliesPlayerTurnQueue.RemoveAt(0);
			}
			else {
				BattleManager->PlayerTurnController();
			}
			ToBattleTransitionScreen->RemoveFromParent();
			if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()); IsValid(PC)) {
				PC->bShowMouseCursor = true;
				PC->bEnableClickEvents = true;
				PC->bEnableMouseOverEvents = true;
			}
		}
}

void AGameManager::EndBattle()
{
	APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
	URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
	UUIManagerWorldSubsystem* UIManagerWorldSubsystem = Cast<UUIManagerWorldSubsystem>(GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>());
	//Move and rotate player to his last location and rotation. Set ViewCamera to player's camera
	FViewTargetTransitionParams Params{};
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if (ABattleManager* BattleManager = RedemptionGameModeBase->GetBattleManager();IsValid(BattleManager))
			if (IsValid(PC) && IsValid(PlayerCharacter) && IsValid(GameInstance) && IsValid(UIManagerWorldSubsystem)) {
				PC->SetViewTarget(PlayerCharacter->GetFollowCamera()->GetOwner(), Params);
				PlayerCharacter->SetActorLocation(PlayerLastLocation);
				PC->bShowMouseCursor = false;
				PC->bEnableClickEvents = false;
				PC->bEnableMouseOverEvents = false;
				UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
				//PC->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
				PlayerCharacter->SetCanInput(true);
				BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
				BattleManager->SetCanTurnBehindPlayerCameraToTarget(false);
				for (int i = BattleManager->BattleEnemies.Num() - 1; i >= 0; i--)
					BattleManager->BattleEnemies[i]->Destroy();							 
				PlayerCharacter->CopyInfoFromCombatPlayer(BattleManager->CombatPlayerCharacter);
				BattleManager->BattleAlliesPlayer.Remove(BattleManager->CombatPlayerCharacter);
				BattleManager->CombatPlayerCharacter->Destroy();
				for (int i = BattleManager->BattleAlliesPlayer.Num() - 1; i >= 0; i--) {
					if (ACombatAllies* const CombatAllies = Cast<ACombatAllies>(BattleManager->BattleAlliesPlayer[i]); IsValid(CombatAllies)) {
						CombatAllies->ResetSkillsLeveledUp();
						CombatAllies->SetActorLocation(FVector(-500.0, -500.0, -500.0));
						BattleManager->BattleAlliesPlayer.RemoveAt(i);
						CombatAllies->GetCapsuleComponent()->SetEnableGravity(false);
					}
				}
				BattleManager->BattleEnemies.Empty();
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->PercentDelegate.Clear();
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->PercentDelegate.Clear();
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesInfoVerticalBoxes()[0]->SetVisibility(ESlateVisibility::Visible);
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->PercentDelegate.BindUFunction(PlayerCharacter, "GetHealthPercentage");
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->PercentDelegate.BindUFunction(PlayerCharacter, "GetManaPercentage");
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->SynchronizeProperties();
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->SynchronizeProperties();
				UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesNameTextBlockes()[0]->SetText(FText::FromString(PlayerCharacter->CharacterName));
				RestartBattleTransitionScreenWidget();
				//Background Music set
				RedemptionGameModeBase->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->SetPaused(true);
				if (IsValid(GameInstance)) {
					bool SelectingMusic = true;
					do {
						int8 RandomBGMusicIndex = FMath::RandRange(0, RedemptionGameModeBase->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponents.Num() - 1);
						if (RandomBGMusicIndex != GameInstance->InstancePreviousCombatBGMusicIndex) {
							RedemptionGameModeBase->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponents[RandomBGMusicIndex]->Play();
							RedemptionGameModeBase->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponents[RandomBGMusicIndex]->SetPaused(false);
							RedemptionGameModeBase->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic = RandomBGMusicIndex;
							GameInstance->InstancePreviousDungeonBGMusicIndex = RandomBGMusicIndex;
							SelectingMusic = false;
						}
					} while (SelectingMusic);
				}
				GameInstance->InstanceKilledEnemies++;
				CurrentNumberOfNonCombatEnemies--;
				static const FString ContextString(TEXT("Characters And Related Quests List Context"));
				if (IsValid(CharactersAndRelatedQuestsDataTable)) {
					FCharactersAndRelatedQuests CharacterAndRelatedQuest{};
					TArray<FCharactersAndRelatedQuests*> CharactersAndQuestsRowsArray{};
					CharactersAndRelatedQuestsDataTable->GetAllRows<FCharactersAndRelatedQuests>(ContextString, CharactersAndQuestsRowsArray);
					for (FCharactersAndRelatedQuests* CharacterAndRelatedQuestRow : CharactersAndQuestsRowsArray)
						if (CharacterAndRelatedQuestRow->CharacterName.IsEqual(AttackingCharacterNPC->GetCharacterName())) {
							CharacterAndRelatedQuest = *CharacterAndRelatedQuestRow;
							break;
						}
					if (IsValid(CharacterAndRelatedQuest.QuestClass))
						if (auto* const QuestManager = RedemptionGameModeBase->GetQuestManager(); IsValid(QuestManager)) {
							bool QuestFound = false;
							for (const auto QuestAndItsStage : QuestManager->GetActiveOrFinishedQuestsAndTheirStages())
								if (QuestAndItsStage.QuestClass == CharacterAndRelatedQuest.QuestClass) {
									QuestManager->AdvanceQuest(CharacterAndRelatedQuest.QuestClass, CharacterAndRelatedQuest.QuestStageIfQuestAlreadyAccepted);
									QuestFound = true;
									break;
								}
							if (!QuestFound)
								QuestManager->ActivateNewQuest(CharacterAndRelatedQuest.QuestClass, CharacterAndRelatedQuest.QuestStageIfQuestIsNotAccepted);
						}
				}
				if (auto* AttackingTownNPCWithBattleEnemies = Cast<ATownNPCWithBattleEnemies>(AttackingCharacterNPC); IsValid(AttackingTownNPCWithBattleEnemies)) {
					if(AttackingTownNPCWithBattleEnemies->GetDestroyAfterBattle())
						AttackingCharacterNPC->Destroy();
				}
				else
					AttackingCharacterNPC->Destroy();
				if (IsValid(ActorToRestoreDialogueWith)){
					PlayerCharacter->DialogueInteract(ActorToRestoreDialogueWith); 
					ActorToRestoreDialogueWith = false;
				}
			}
}

void AGameManager::EnemySpawn()
{
	if (CurrentNumberOfNonCombatEnemies < MaxNumberOfNonCombatEnemies && SpawnBoundingBoxes.Num() > 0 && NonCombatEnemyNPCClasses.Num() > 0 && CombatEnemiesVariants.Num() > 0) {
		uint8 RandomSpawnIndex = FMath::RandRange(0, SpawnBoundingBoxes.Num() - 1);
		uint8 RandomEnemyClassIndex = FMath::RandRange(0, NonCombatEnemyNPCClasses.Num() - 1);
		FVector BoxCenter{};
		FVector BoxExtends{};
		SpawnBoundingBoxes[RandomSpawnIndex]->GetActorBounds(false, BoxCenter, BoxExtends);
		FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(BoxCenter, BoxExtends);
		FTransform SpawnTransform{};
		SpawnTransform.SetLocation(SpawnLocation);
		SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());
		ANonCombatEnemyNPC* NonCombatEnemyNPC = GetWorld()->SpawnActor<ANonCombatEnemyNPC>(NonCombatEnemyNPCClasses[RandomEnemyClassIndex], SpawnTransform, FActorSpawnParameters::FActorSpawnParameters());
		if (IsValid(NonCombatEnemyNPC)) {
			ASmartObject* NonCombatEnemyNPCSmartObject = GetWorld()->SpawnActor<ASmartObject>(NonCombatEnemyNPCSmartObjectClass);
			NonCombatEnemyNPC->SetSmartObject(NonCombatEnemyNPCSmartObject);
			if (ANonCombatEnemyNPCAIController* AIController = Cast<ANonCombatEnemyNPCAIController>(NonCombatEnemyNPC->GetController()); IsValid(AIController))
				AIController->SetDynamicSubtree();
			uint8 RandomEnemiesVariant = FMath::RandRange(0, CombatEnemiesVariants.Num() - 1);
			NonCombatEnemyNPC->SetBattleEnemies(CombatEnemiesVariants[RandomEnemiesVariant].CombatEnemiesVariant);
		}
		CurrentNumberOfNonCombatEnemies++;
	}
	FTimerHandle EnemySpawnTimerHandle{};
	GetWorld()->GetTimerManager().SetTimer(EnemySpawnTimerHandle, this, &AGameManager::EnemySpawn, EnemySpawnInterval, false);
}

void AGameManager::RestartBattleTransitionScreenWidget()
{

}

TArray<AActor*> AGameManager::GetEnemyBattleSpawns() const
{
	return EnemyBattleSpawns;
}

TArray<ACombatStartLocation*> AGameManager::GetAlliesPlayerBattleSpawns() const
{
	return AlliesPlayerBattleSpawns;
}




