// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include <Kismet/GameplayStatics.h>
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\ToBattleTransitionScreen.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\NonCombatEnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include "Components/StackBox.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\RedemptionGameInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\CombatEnemyNPCAIController.h"

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
		UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *AttackingNPC->GetName());
		PlayerLastLocation = PlayerCharacter->GetActorLocation();
		PlayerLastRotation = PlayerCharacter->GetActorRotation();
		//Set player's last rotation and location, teleport player to battle scene, destroy roaming NPC, disable input and battle UI widgets
		if (APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController()); IsValid(PC)) {
			PC->bShowMouseCursor = true;
			PC->bEnableClickEvents = true;
			PC->bEnableMouseOverEvents = true;
			PC->ActivateTouchInterface(PlayerCharacter->GetEmptyTouchInterface());
			PlayerCharacter->SetCanInput(false);
			PlayerCharacter->SetActorLocation(PlayerBattleSpawn->GetActorLocation());
			PlayerCharacter->SetActorRotation(FRotator(0, 180, 0));
			FViewTargetTransitionParams Params;
			PC->SetViewTarget(PlayerCharacter->GetBattleManager()->GetBehindPlayerCamera(), Params);
			ToBattleTransitionScreen->AddToViewport(0);
			ToBattleTransitionScreen->SetMainTextBlockText(FText::FromString("Initiative!"));
			GetWorld()->GetTimerManager().SetTimer(ToBattleTransitionTimerHandle, this, &AGameManager::ToBattleTransition, 2.f, false);
			//Create enemies and add them to BattleEnemies array in battle manager
			if (ANonCombatEnemyNPC* NonCombatEnemyNPC = Cast<ANonCombatEnemyNPC>(AttackingNPC); IsValid(NonCombatEnemyNPC))
				for (uint8 i = 0; i < NonCombatEnemyNPC->GetBattleEnemies().Num(); i++) {
					const FVector Location = EnemyBattleSpawns[i]->GetActorLocation();
					ACombatEnemyNPC* SpawnedEnemy = GetWorld()->SpawnActor<ACombatEnemyNPC>(NonCombatEnemyNPC->GetBattleEnemies()[i], Location, EnemyBattleSpawns[i]->GetActorRotation());
					AWarzombieBattleAI* AISmartObject = GetWorld()->SpawnActor<AWarzombieBattleAI>(AWarzombieBattleAIClass);
					if (IsValid(SpawnedEnemy) && IsValid(AISmartObject))
						SpawnedEnemy->SetSmartObject(AISmartObject);
					ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(SpawnedEnemy->GetController());
					if (IsValid(AIController))
						AIController->SetDynamicSubtree();
					if (IsValid(BattleManager)) {
						SpawnedEnemy->StartLocation = EnemyBattleSpawns[i];
						BattleManager->BattleEnemies.Add(SpawnedEnemy);
						BattleManager->BattleActors.Add(SpawnedEnemy);
					}
				}
			//Background Music set
			PlayerCharacter->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponent->SetPaused(true);
			PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponent->SetPaused(false);
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
		PlayerCharacter->SetActorRotation(PlayerLastRotation);
		PlayerCharacter->SetActorLocation(PlayerLastLocation);
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
		PC->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
		PlayerCharacter->SetCanInput(true);
		PlayerCharacter->GetBattleResultsScreenWidget()->RemoveFromParent();
		BattleManager->SetCanTurnBehindPlayerCameraToStartPosition(false);
		BattleManager->SetCanTurnBehindPlayerCameraToEnemy(false);
		for (int i = 0; i < BattleManager->BattleActors.Num(); i++)
			BattleManager->BattleActors[i]->Destroy();
		BattleManager->BattleActors.Empty();
		PlayerCharacter->RestartBattleMenuWidget();
		PlayerCharacter->RestartBattleResultsScreenWidget();
		RestartBattleTransitionScreenWidget();
		//Background Music set
		PlayerCharacter->GetAudioManager()->DungeonBattleResultsBackgroundMusicAudioComponent->SetPaused(true);
		PlayerCharacter->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponent->SetPaused(false);
		GameInstance->KilledEnemies++;
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

AActor* AGameManager::GetPlayerBattleSpawn() const
{
	return PlayerBattleSpawn;
}

TArray<AActor*> AGameManager::GetEnemyBattleSpawns() const
{
	return EnemyBattleSpawns;
}




