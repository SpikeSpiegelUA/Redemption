// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include <Kismet/GameplayStatics.h>
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\ToBattleTransitionScreen.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\NonCombat\NonCombatEnemyNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Combat\CombatAllyNPC.h"
#include "Components/StackBox.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ArrayActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\Combat\CombatEnemyNPCAIController.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ArrayActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\Combat\CombatAllyNPCAIController.h"

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
void AGameManager::StartBattle(AActor* const& AttackingNPC)
{
	if (IsValid(PlayerCharacter)) {
		PlayerLastLocation = PlayerCharacter->GetActorLocation();
		//Set player's last rotation and location, teleport player to battle scene, destroy roaming NPC, disable input and battle UI widgets
		if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()); IsValid(PC)) {
			ToBattleTransitionScreen->AddToViewport(0);
			ToBattleTransitionScreen->SetMainTextBlockText(FText::FromString("Initiative!"));
			GetWorld()->GetTimerManager().SetTimer(ToBattleTransitionTimerHandle, this, &AGameManager::ToBattleTransition, 2.f, false);
			//Create enemies and add them to BattleEnemies array in battle manager
			TArray<AActor*> ShuffledEnemyBattleSpawns = EnemyBattleSpawns;
			ArrayActions::ShuffleArray(ShuffledEnemyBattleSpawns);
			if (ANonCombatEnemyNPC* NonCombatEnemyNPC = Cast<ANonCombatEnemyNPC>(AttackingNPC); IsValid(NonCombatEnemyNPC))
				for (uint8 i = 0; i < NonCombatEnemyNPC->GetBattleEnemies().Num(); i++) {
					const FVector Location = ShuffledEnemyBattleSpawns[i]->GetActorLocation();
					ACombatEnemyNPC* SpawnedEnemy = GetWorld()->SpawnActor<ACombatEnemyNPC>(NonCombatEnemyNPC->GetBattleEnemies()[i], Location, ShuffledEnemyBattleSpawns[i]->GetActorRotation());
					ASmartObject* AISmartObject = GetWorld()->SpawnActor<ASmartObject>(SpawnedEnemy->GetAIClass());
					if (IsValid(SpawnedEnemy) && IsValid(AISmartObject))
						SpawnedEnemy->SetSmartObject(AISmartObject);
					ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(SpawnedEnemy->GetController());
					SpawnedEnemy->SetStartLocation(ShuffledEnemyBattleSpawns[i]);
					if (IsValid(AIController))
						AIController->SetDynamicSubtree();
					if (IsValid(BattleManager)) {
					//	SpawnedEnemy->SetStartLocation(ShuffledEnemyBattleSpawns[i]);
						BattleManager->BattleEnemies.Add(SpawnedEnemy);
					}
				}
			//Create allies and combat player character
			TArray<ACombatStartLocation*> ShuffledAlliesBattleSpawns = AlliesPlayerBattleSpawns;
			ArrayActions::ShuffleArray(ShuffledAlliesBattleSpawns);
			for (uint8 i = 0; i < PlayerCharacter->GetAllies().Num(); i++) {
				const FVector Location = ShuffledAlliesBattleSpawns[i]->GetActorLocation();
				ACombatAllyNPC* SpawnedAlly = GetWorld()->SpawnActor<ACombatAllyNPC>(PlayerCharacter->GetAllies()[i], Location, FRotator(0, 180, 0));
				ASmartObject* AISmartObject = GetWorld()->SpawnActor<ASmartObject>(SpawnedAlly->GetAIClass());
				if (IsValid(SpawnedAlly) && IsValid(AISmartObject))
					SpawnedAlly->SetSmartObject(AISmartObject);
				ACombatAllyNPCAIController* AIController = Cast<ACombatAllyNPCAIController>(SpawnedAlly->GetController());
				if (IsValid(AIController))
					AIController->SetDynamicSubtree();
				if (IsValid(BattleManager)) {
					SpawnedAlly->SetStartLocation(ShuffledAlliesBattleSpawns[i]);
					SpawnedAlly->SetActorLocation(ShuffledAlliesBattleSpawns[i]->GetActorLocation());
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
			CombatPlayerCharacter->SetActorLocation(ShuffledAlliesBattleSpawns[3]->GetActorLocation());
			CombatPlayerCharacter->SetActorRotation(FRotator(0, 180, 0));
			CombatPlayerCharacter->SetStartLocation(ShuffledAlliesBattleSpawns[3]);
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesHealthBars()[BattleManager->BattleAlliesPlayer.Num()]->PercentDelegate.Clear();
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesManaBars()[BattleManager->BattleAlliesPlayer.Num()]->PercentDelegate.Clear();
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesInfoVerticalBoxes()[BattleManager->BattleAlliesPlayer.Num()]->SetVisibility(ESlateVisibility::Visible);
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesHealthBars()[BattleManager->BattleAlliesPlayer.Num()]->PercentDelegate.BindUFunction(CombatPlayerCharacter, "GetHealthPercentage");
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesManaBars()[BattleManager->BattleAlliesPlayer.Num()]->PercentDelegate.BindUFunction(CombatPlayerCharacter, "GetManaPercentage");
			PlayerCharacter->GetAlliesInfoBarsWidget()->GetAlliesNameTextBlockes()[BattleManager->BattleAlliesPlayer.Num()]->SetText(FText::FromName(CombatPlayerCharacter->GetCharacterName()));
			BattleManager->BattleAlliesPlayer.Add(CombatPlayerCharacter);
			BattleManager->CombatPlayerCharacter = CombatPlayerCharacter;
			BattleManager->AlliesPlayerTurnQueue.Add(BattleManager->BattleAlliesPlayer.Num() - 1);
			ArrayActions::ShuffleArray<int>(BattleManager->AlliesPlayerTurnQueue);
			//Set camera position and set non combat player position to PlayerPlane's position
			PlayerCharacter->GetBattleManager()->CurrentTurnAllyPlayerIndex = BattleManager->AlliesPlayerTurnQueue[0];
			PlayerCharacter->GetBattleManager()->SetBehindPlayerCameraLocation(Cast<ACombatStartLocation>(BattleManager->BattleAlliesPlayer[BattleManager->AlliesPlayerTurnQueue[0]]->GetStartLocation())->CombatCameraLocation);
			PlayerCharacter->SetCanInput(false);
			PlayerCharacter->SetActorLocation(FVector(1350, 5610, -960));
			PlayerCharacter->GetAlliesInfoBarsWidget()->AddToViewport();
			BattleManager->AlliesPlayerTurnQueue.RemoveAt(0);
			PC->bShowMouseCursor = true;
			PC->bEnableClickEvents = true;
			PC->bEnableMouseOverEvents = true;
			PC->ActivateTouchInterface(PlayerCharacter->GetEmptyTouchInterface());
			FViewTargetTransitionParams Params;
			PC->SetViewTarget(PlayerCharacter->GetBattleManager()->GetBehindPlayerCamera(), Params);
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
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
		PC->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
		PlayerCharacter->SetCanInput(true);
		PlayerCharacter->GetBattleResultsScreenWidget()->RemoveFromParent();
		BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
		BattleManager->SetCanTurnBehindPlayerCameraToTarget(false);
		for (int i = 0; i < BattleManager->BattleEnemies.Num(); i++) {
			PlayerCharacter->Gold += BattleManager->BattleEnemies[i]->GetGoldReward();
			BattleManager->BattleEnemies[i]->Destroy();
		}
		GameInstance->InstanceGold = PlayerCharacter->Gold;
		BattleManager->BattleEnemies.Empty();
		PlayerCharacter->RestartBattleMenuWidget();
		PlayerCharacter->RestartBattleResultsScreenWidget();
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
	TArray<AActor*> UIManagerActors;
	AUIManager* UIManager = nullptr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUIManager::StaticClass(), UIManagerActors);
	if (UIManagerActors.Num() > 0)
		UIManager = Cast<AUIManager>(UIManagerActors[0]);
	ToBattleTransitionScreen->RemoveFromParent();
	if (IsValid(BattleMenu) && IsValid(UIManager)) {
		BattleMenu->AddToViewport();
		BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
		UIManager->PickedButton = BattleMenu->GetAttackButton();
		UIManager->PickedButtonIndex = 0;
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




