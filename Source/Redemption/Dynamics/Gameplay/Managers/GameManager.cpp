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
	TArray<AActor*> BattleManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleManager::StaticClass(), BattleManagerActors);
	if (BattleManagerActors.Num() > 0)
		BattleManager = Cast<ABattleManager>(BattleManagerActors[0]);
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Set up battle stage
void AGameManager::StartBattle(AActor* const AttackingNPC)
{
	if (IsValid(PlayerCharacter) && IsValid(BattleManager)) {
		PlayerLastLocation = PlayerCharacter->GetActorLocation();
		//Set player's last rotation and location, teleport player to battle scene, destroy roaming NPC, disable input and battle UI widgets
		if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()); IsValid(PC)) {
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
					if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(SpawnedEnemy->GetController()); IsValid(AIController))
						AIController->SetDynamicSubtree();
					BattleManager->BattleEnemies.Add(SpawnedEnemy);
				}
			}
			//Create allies and combat player character
			TArray<TSubclassOf<ACombatAllies>> ShuffledCombatAllies = PlayerCharacter->GetAllies();
			ArrayActions::ShuffleArray(ShuffledCombatAllies);
			PlayerCharacter->SetAllies(ShuffledCombatAllies);
			for (uint8 i = 0; i < PlayerCharacter->GetAllies().Num() && i < 4; i++) {
				const FVector Location = AlliesPlayerBattleSpawns[i]->GetActorLocation();
				ACombatAllyNPC* SpawnedAlly = GetWorld()->SpawnActor<ACombatAllyNPC>(PlayerCharacter->GetAllies()[i], Location, FRotator(0, 180, 0));
				if (IsValid(SpawnedAlly->GetAIClass())) 
					if (ASmartObject* AISmartObject = GetWorld()->SpawnActor<ASmartObject>(SpawnedAlly->GetAIClass()); IsValid(SpawnedAlly) && IsValid(AISmartObject))
						SpawnedAlly->SetSmartObject(AISmartObject);
				ACombatAllyNPCAIController* AIController = Cast<ACombatAllyNPCAIController>(SpawnedAlly->GetController());
				if (IsValid(AIController))
					AIController->SetDynamicSubtree();
				if (IsValid(BattleManager)) {
					SpawnedAlly->SetStartLocation(AlliesPlayerBattleSpawns[i]);
					SpawnedAlly->SetActorLocation(AlliesPlayerBattleSpawns[i]->GetActorLocation());
					BattleManager->BattleAlliesPlayer.Add(SpawnedAlly);
					BattleManager->AlliesPlayerTurnQueue.Add(i);
				}
				PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesHealthBars()[i]->PercentDelegate.Clear();
				PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesManaBars()[i]->PercentDelegate.Clear();
				PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesInfoVerticalBoxes()[i]->SetVisibility(ESlateVisibility::Visible);
				PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesHealthBars()[i]->PercentDelegate.BindUFunction(SpawnedAlly, "GetHealthPercentage");
				PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesManaBars()[i]->PercentDelegate.BindUFunction(SpawnedAlly, "GetManaPercentage");
				PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesNameTextBlockes()[i]->SetText(FText::FromName(SpawnedAlly->GetCharacterName()));
			}
			ACombatPlayerCharacter* CombatPlayerCharacter = GetWorld()->SpawnActor<ACombatPlayerCharacter>(CombatPlayerCharacterClass);
			CombatPlayerCharacter->SetActorLocation(AlliesPlayerBattleSpawns[3]->GetActorLocation());
			CombatPlayerCharacter->SetActorRotation(FRotator(0, 180, 0));
			CombatPlayerCharacter->SetStartLocation(AlliesPlayerBattleSpawns[3]);
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesHealthBars()[BattleManager->BattleAlliesPlayer.Num()]->PercentDelegate.Clear();
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesManaBars()[BattleManager->BattleAlliesPlayer.Num()]->PercentDelegate.Clear();
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesInfoVerticalBoxes()[BattleManager->BattleAlliesPlayer.Num()]->SetVisibility(ESlateVisibility::Visible);
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesHealthBars()[BattleManager->BattleAlliesPlayer.Num()]->PercentDelegate.BindUFunction(CombatPlayerCharacter, "GetHealthPercentage");
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesManaBars()[BattleManager->BattleAlliesPlayer.Num()]->PercentDelegate.BindUFunction(CombatPlayerCharacter, "GetManaPercentage");
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesNameTextBlockes()[BattleManager->BattleAlliesPlayer.Num()]->SetText(FText::FromName(CombatPlayerCharacter->GetCharacterName()));
			BattleManager->BattleAlliesPlayer.Add(CombatPlayerCharacter);
			BattleManager->CombatPlayerCharacter = CombatPlayerCharacter;
			BattleManager->AlliesPlayerTurnQueue.Add(BattleManager->BattleAlliesPlayer.Num() - 1);
			ArrayActions::ShuffleArray<uint8>(BattleManager->AlliesPlayerTurnQueue);
			//Set camera position and set non combat player position to PlayerPlane's position
			BattleManager->CurrentTurnCombatNPCIndex = BattleManager->AlliesPlayerTurnQueue[0];
			BattleManager->SetBehindPlayerCameraLocation(Cast<ACombatStartLocation>(BattleManager->BattleAlliesPlayer[BattleManager->AlliesPlayerTurnQueue[0]]->GetStartLocation())->CombatCameraLocation);
			PlayerCharacter->SetCanInput(false);
			PlayerCharacter->SetActorLocation(FVector(1350, 5610, -960));
			PlayerCharacter->GetAlliesInfoBarsWidget()->AddToViewport();
			PlayerCharacter->GetSkillBattleMenuWidget()->ResetSkillsScrollBox();
			for (TSubclassOf<ASpell> SpellClass : BattleManager->BattleAlliesPlayer[BattleManager->CurrentTurnCombatNPCIndex]->GetAvailableSkills())
				PlayerCharacter->GetSkillBattleMenuWidget()->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
			if (UAlliesInfoBars* AlliesInfoBarsWidget = PlayerCharacter->GetAlliesInfoBarsWidget(); IsValid(AlliesInfoBarsWidget)) {
				if(AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder < AlliesInfoBarsWidget->GetAlliesNameBorders().Num())
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
			PC->SetViewTarget(PlayerCharacter->GetBattleManager()->GetBehindPlayerCamera(), Params);
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC, PlayerCharacter->GetBattleMenuWidget(), EMouseLockMode::DoNotLock);
			//Background Music set
			PlayerCharacter->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
			URedemptionGameInstance* GameInstance = Cast<URedemptionGameInstance>(GetWorld()->GetGameInstance());
			if (IsValid(GameInstance)) {
				bool SelectingMusic = true;
				do {
					int8 RandomBGMusicIndex = FMath::RandRange(0, PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents.Num() - 1);
					if (RandomBGMusicIndex != GameInstance->InstancePreviousCombatBGMusicIndex) {
						PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[RandomBGMusicIndex]->Play();
						PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[RandomBGMusicIndex]->SetPaused(false);
						PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic = RandomBGMusicIndex;
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
	//Move and rotate player to his last location and rotation. Set ViewCamera to player's camera
	FViewTargetTransitionParams Params{};
	if (IsValid(PC) && IsValid(PlayerCharacter) && IsValid(GameInstance) && IsValid(BattleManager)) {
		PC->SetViewTarget(PlayerCharacter->GetFollowCamera()->GetOwner(), Params);
		PlayerCharacter->SetActorLocation(PlayerLastLocation);
		PC->bShowMouseCursor = false;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
		//PC->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
		PlayerCharacter->SetCanInput(true);
		PlayerCharacter->GetBattleResultsScreenWidget()->RemoveFromParent();
		BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
		BattleManager->SetCanTurnBehindPlayerCameraToTarget(false);
		for (int i = BattleManager->BattleEnemies.Num() - 1; i >= 0; i--) {
			PlayerCharacter->Gold += BattleManager->BattleEnemies[i]->GetGoldReward();
			BattleManager->BattleEnemies[i]->Destroy();
		}
		PlayerCharacter->CurrentHP = BattleManager->CombatPlayerCharacter->CurrentHP;
		PlayerCharacter->CurrentMana = BattleManager->CombatPlayerCharacter->CurrentMana;
		for (int i = BattleManager->BattleAlliesPlayer.Num() - 1; i >= 0; i--)
			BattleManager->BattleAlliesPlayer[i]->Destroy();
		BattleManager->BattleEnemies.Empty();
		PlayerCharacter->RestartBattleMenuWidget();
		PlayerCharacter->RestartBattleResultsScreenWidget();
		PlayerCharacter->GetAlliesInfoBarsWidget()->RemoveFromParent();
		RestartBattleTransitionScreenWidget();
		//Background Music set
		PlayerCharacter->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->SetPaused(true);
		if (IsValid(GameInstance)) {
			bool SelectingMusic = true;
			do {
				int8 RandomBGMusicIndex = FMath::RandRange(0, PlayerCharacter->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponents.Num() - 1);
				if (RandomBGMusicIndex != GameInstance->InstancePreviousCombatBGMusicIndex) {
					PlayerCharacter->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponents[RandomBGMusicIndex]->Play();
					PlayerCharacter->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponents[RandomBGMusicIndex]->SetPaused(false);
					PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic = RandomBGMusicIndex;
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
	UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget();
	ToBattleTransitionScreen->RemoveFromParent();
	if (UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(BattleMenu) && IsValid(UIManagerWorldSubsystem)) {
		BattleMenu->AddToViewport();
		BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManagerWorldSubsystem->PickedButton = BattleMenu->GetAttackButton();
		UIManagerWorldSubsystem->PickedButtonIndex = 0;
		BattleMenu->IsChoosingAction = true;
	}
}

void AGameManager::RestartBattleTransitionScreenWidget()
{

}

ABattleManager* AGameManager::GetBattleManager() const
{
	return BattleManager;
}

TArray<AActor*> AGameManager::GetEnemyBattleSpawns() const
{
	return EnemyBattleSpawns;
}

TArray<ACombatStartLocation*> AGameManager::GetAlliesPlayerBattleSpawns() const
{
	return AlliesPlayerBattleSpawns;
}




