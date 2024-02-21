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
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Set up battle stage
void AGameManager::StartBattle(AActor* const AttackingNPC)
{
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
					if (IsValid(UIManagerWorldSubsystem->BattleMenuWidget))
						UIManagerWorldSubsystem->BattleMenuWidget->AddToViewport();
					ToBattleTransitionScreen->AddToViewport(0);
					ToBattleTransitionScreen->SetMainTextBlockText(FText::FromString("Initiative!"));
					GetWorld()->GetTimerManager().SetTimer(ToBattleTransitionTimerHandle, this, &AGameManager::ToBattleTransition, 2.f, false);
					//Create enemies and add them to BattleEnemies array in battle manager
					if (ANonCombatEnemyNPC* NonCombatEnemyNPC = Cast<ANonCombatEnemyNPC>(AttackingNPC); IsValid(NonCombatEnemyNPC)) {
						TArray<TSubclassOf<ACombatEnemyNPC>> ShuffledNPCBattleEnemies = NonCombatEnemyNPC->GetBattleEnemies();
						ArrayActions::ShuffleArray(ShuffledNPCBattleEnemies);
						NonCombatEnemyNPC->SetBattleEnemies(ShuffledNPCBattleEnemies);
						for (uint8 i = 0; i < NonCombatEnemyNPC->GetBattleEnemies().Num() && i < 4; i++) {
							const FVector Location = EnemyBattleSpawns[i]->GetActorLocation();
							ACombatEnemyNPC* SpawnedEnemy = GetWorld()->SpawnActor<ACombatEnemyNPC>(NonCombatEnemyNPC->GetBattleEnemies()[i], Location, EnemyBattleSpawns[i]->GetActorRotation());
							ASmartObject* AISmartObject = GetWorld()->SpawnActor<ASmartObject>(SpawnedEnemy->GetAIClass());
							if (IsValid(SpawnedEnemy) && IsValid(AISmartObject))
								SpawnedEnemy->SetSmartObject(AISmartObject);
							SpawnedEnemy->SetStartLocation(EnemyBattleSpawns[i]);
							SpawnedEnemy->SetStartRotation(SpawnedEnemy->GetActorRotation());
							if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(SpawnedEnemy->GetController()); IsValid(AIController))
								AIController->SetDynamicSubtree();
							BattleManager->BattleEnemies.Add(SpawnedEnemy);
						}
					}
					//Create allies and combat player character
					TArray<ACombatAllies*> ShuffledCombatAllies{};
					for (ACombatAllyNPC* CombatAllyNPC : PlayerCharacter->GetAllies())
						ShuffledCombatAllies.Add(CombatAllyNPC);
					ArrayActions::ShuffleArray(ShuffledCombatAllies);
					for (ACombatAllies* CombatAllies : ShuffledCombatAllies)
						PlayerCharacter->GetAllies().Add(Cast<ACombatAllyNPC>(CombatAllies));
					for (uint8 i = 0; i < PlayerCharacter->GetAllies().Num() && i < 4; i++) {
						if (IsValid(PlayerCharacter->GetAllies()[i])) {
							const FVector Location = AlliesPlayerBattleSpawns[i]->GetActorLocation();
							PlayerCharacter->GetAllies()[i]->SetActorLocation(Location);
							PlayerCharacter->GetAllies()[i]->SetActorRotation(FRotator(0.0, 180.0, 0.0));
							PlayerCharacter->GetAllies()[i]->SetStartLocation(AlliesPlayerBattleSpawns[i]);
							PlayerCharacter->GetAllies()[i]->SetStartRotation(PlayerCharacter->GetAllies()[i]->GetActorRotation());
							if (IsValid(PlayerCharacter->GetAllies()[i]->GetAIClass()))
								if (ASmartObject* AISmartObject = GetWorld()->SpawnActor<ASmartObject>(PlayerCharacter->GetAllies()[i]->GetAIClass()); IsValid(AISmartObject))
									PlayerCharacter->GetAllies()[i]->SetSmartObject(AISmartObject);
							ACombatAllyNPCAIController* AIController = Cast<ACombatAllyNPCAIController>(PlayerCharacter->GetAllies()[i]->GetController());
							if (IsValid(AIController))
								AIController->SetDynamicSubtree();
							if (IsValid(BattleManager)) {
								BattleManager->BattleAlliesPlayer.Add(PlayerCharacter->GetAllies()[i]);
								BattleManager->AlliesPlayerTurnQueue.Add(i);
							}
						}
					}
					ACombatPlayerCharacter* CombatPlayerCharacter = GetWorld()->SpawnActor<ACombatPlayerCharacter>(CombatPlayerCharacterClass);
					CombatPlayerCharacter->SetActorLocation(AlliesPlayerBattleSpawns[3]->GetActorLocation());
					CombatPlayerCharacter->SetActorRotation(FRotator(0, 180, 0));
					CombatPlayerCharacter->SetStartLocation(AlliesPlayerBattleSpawns[3]);
					CombatPlayerCharacter->SetStartRotation(CombatPlayerCharacter->GetActorRotation());
					BattleManager->BattleAlliesPlayer.Add(CombatPlayerCharacter);
					BattleManager->CombatPlayerCharacter = CombatPlayerCharacter;
					BattleManager->AlliesPlayerTurnQueue.Add(BattleManager->BattleAlliesPlayer.Num() - 1);
					UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->PercentDelegate.Clear();
					UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->PercentDelegate.Clear();
					UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesInfoVerticalBoxes()[0]->SetVisibility(ESlateVisibility::Visible);
					UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->PercentDelegate.BindUFunction(CombatPlayerCharacter, "GetHealthPercentage");
					UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->PercentDelegate.BindUFunction(CombatPlayerCharacter, "GetManaPercentage");
					UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesHealthBars()[0]->SynchronizeProperties();
					UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesManaBars()[0]->SynchronizeProperties();
					UIManagerWorldSubsystem->AlliesInfoBarsWidget->GetAlliesNameTextBlockes()[0]->SetText(FText::FromName(CombatPlayerCharacter->GetCharacterName()));
					ArrayActions::ShuffleArray<uint8>(BattleManager->AlliesPlayerTurnQueue);
					//Set camera position and set non combat player position to PlayerPlane's position
					BattleManager->CurrentTurnCombatNPCIndex = BattleManager->AlliesPlayerTurnQueue[0];
					BattleManager->SetBehindPlayerCameraLocation(Cast<ACombatStartLocation>(BattleManager->BattleAlliesPlayer[BattleManager->AlliesPlayerTurnQueue[0]]->GetStartLocation())->CombatCameraLocation);
					PlayerCharacter->SetCanInput(false);
					PlayerCharacter->SetActorLocation(FVector(1350, 5610, -960));
					UIManagerWorldSubsystem->AlliesInfoBarsWidget->AddToViewport();
					UIManagerWorldSubsystem->SkillBattleMenuWidget->ResetSkillsScrollBox();
					for (TSubclassOf<ASpell> SpellClass : BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnCombatNPCIndex]->GetAvailableSpells())
						UIManagerWorldSubsystem->SkillBattleMenuWidget->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
					if (UAlliesInfoBars* AlliesInfoBarsWidget = UIManagerWorldSubsystem->AlliesInfoBarsWidget; IsValid(AlliesInfoBarsWidget)) {
						if (AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder < AlliesInfoBarsWidget->GetAlliesNameBorders().Num())
							AlliesInfoBarsWidget->GetAlliesNameBorders()[AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder]->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
						AlliesInfoBarsWidget->GetAlliesNameBorders()[BattleManager->CurrentTurnCombatNPCIndex]->SetBrushColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
						AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder = BattleManager->CurrentTurnCombatNPCIndex;
					}
					BattleManager->AlliesPlayerTurnQueue.RemoveAt(0);
					PC->bShowMouseCursor = true;
					PC->bEnableClickEvents = true;
					PC->bEnableMouseOverEvents = true;
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
					AttackingNPC->Destroy();
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
				UIManagerWorldSubsystem->BattleResultsScreenWidget->RemoveFromParent();
				BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
				BattleManager->SetCanTurnBehindPlayerCameraToTarget(false);
				for (int i = BattleManager->BattleEnemies.Num() - 1; i >= 0; i--)
					BattleManager->BattleEnemies[i]->Destroy();							 
				PlayerCharacter->CopyInfoFromCombatPlayer(BattleManager->CombatPlayerCharacter);
				BattleManager->CombatPlayerCharacter = nullptr;
				for (int i = BattleManager->BattleAlliesPlayer.Num() - 1; i >= 0; i--) {
					if (IsValid(Cast<ACombatPlayerCharacter>(BattleManager->BattleAlliesPlayer[i])))
						BattleManager->BattleAlliesPlayer[i]->Destroy();
					else if (ACombatAllies* const CombatAllies = Cast<ACombatAllies>(BattleManager->BattleAlliesPlayer[i]); IsValid(CombatAllies)) {
						CombatAllies->ResetSkillsLeveledUp();
						CombatAllies->SetActorLocation(FVector(-500.0, -500.0, -500.0));
						BattleManager->BattleAlliesPlayer.RemoveAt(i);
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
			}
}

void AGameManager::ToBattleTransition() 
{
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		ToBattleTransitionScreen->RemoveFromParent();
		if (IsValid(UIManagerWorldSubsystem->BattleMenuWidget)) {
			UIManagerWorldSubsystem->BattleMenuWidget->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			UIManagerWorldSubsystem->PickedButton = UIManagerWorldSubsystem->BattleMenuWidget->GetAttackButton();
			UIManagerWorldSubsystem->PickedButtonIndex = 0;
			UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingAction = true;
		}
	}
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




