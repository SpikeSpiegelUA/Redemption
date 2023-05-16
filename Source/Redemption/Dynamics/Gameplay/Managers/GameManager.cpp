// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include <Kismet/GameplayStatics.h>
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Screens\ToBattleTransitionScreen.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\NonCombatEnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include "Components/StackBox.h"
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
void AGameManager::StartBattle(AActor* AttackingNPC)
{
	PlayerLastLocation = PlayerCharacter->GetActorLocation();
	PlayerLastRotation = PlayerCharacter->GetActorRotation();
	//Teleport player to battle scene, destroy roaming NPC, disable input and battle UI widgets
	APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
	PC->bShowMouseCursor = true;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;
	PC->ActivateTouchInterface(PlayerCharacter->GetEmptyTouchInterface());
	PlayerCharacter->SetCanInput(false);
	PlayerCharacter->SetActorLocation(PlayerBattleSpawn->GetActorLocation());
	PlayerCharacter->SetActorRotation(FRotator(0, 180, 0 ));
	FViewTargetTransitionParams Params;
	PC->SetViewTarget(PlayerCharacter->GetBattleManager()->GetBehindPlayerCamera(), Params);
	AttackingNPC->Destroy();
	ToBattleTransitionScreen->AddToViewport(0);
	ToBattleTransitionScreen->SetMainTextBlockText(FText::FromString("Initiative!"));
	GetWorld()->GetTimerManager().SetTimer(ToBattleTransitionTimerHandle, this, &AGameManager::ToBattleTransition, 2.f, false);
	ANonCombatEnemyNPC* NonCombatEnemyNPC = Cast<ANonCombatEnemyNPC>(AttackingNPC);
	//Create enemies and add them to BattleEnemies array in battle manager
	for (uint8 i = 0; i < NonCombatEnemyNPC->GetBattleEnemies().Num(); i++) {
		const FVector Location = EnemyBattleSpawns[i]->GetActorLocation();
		ACombatEnemyNPC* SpawnedEnemy = GetWorld()->SpawnActor<ACombatEnemyNPC>(NonCombatEnemyNPC->GetBattleEnemies()[i], Location, EnemyBattleSpawns[i]->GetActorRotation());
		AWarzombieBattleAI* AISmartObject = GetWorld()->SpawnActor<AWarzombieBattleAI>(AWarzombieBattleAIClass);
		if(SpawnedEnemy&&AISmartObject)
		SpawnedEnemy->SetSmartObject(AISmartObject);
		ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(SpawnedEnemy->GetController());
		if (AIController)
			AIController->SetDynamicSubtree();
		if (BattleManager) {
			SpawnedEnemy->StartLocation = EnemyBattleSpawns[i];
			BattleManager->BattleEnemies.Add(SpawnedEnemy);
			BattleManager->BattleActors.Add(SpawnedEnemy);
		}
	}
	//Background Music set
	PlayerCharacter->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponent->SetPaused(true);
	PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponent->SetPaused(false);
}

void AGameManager::EndBattle()
{
	//Move and rotate player to his last location and rotation. Set ViewCamera to player's camera
	APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
	FViewTargetTransitionParams Params;
	PC->SetViewTarget(PlayerCharacter->GetFollowCamera()->GetOwner(), Params);
	PlayerCharacter->SetActorRotation(PlayerLastRotation);
	PlayerCharacter->SetActorLocation(PlayerLastLocation);
	PC->bShowMouseCursor = false;
	PC->bEnableClickEvents = false;
	PC->bEnableMouseOverEvents = false;
	PC->ActivateTouchInterface(PlayerCharacter->GetStandardTouchInterface());
	PlayerCharacter->SetCanInput(true);
	PlayerCharacter->GetBattleResultsScreenWidget()->RemoveFromParent();
	for (int i = 0; i < BattleManager->BattleActors.Num(); i++)
		BattleManager->BattleActors[i]->Destroy();
	BattleManager->BattleActors.Empty();
	PlayerCharacter->RestartBattleMenuWidget();
	PlayerCharacter->RestartBattleResultsScreenWidget();
	RestartBattleTransitionScreenWidget();
	//Background Music set
	PlayerCharacter->GetAudioManager()->DungeonBattleResultsBackgroundMusicAudioComponent->SetPaused(true);
	PlayerCharacter->GetAudioManager()->DungeonExplorationBackgroundMusicAudioComponent->SetPaused(false);
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
	BattleMenu->AddToViewport();
	BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
	UIManager->PickedButton = BattleMenu->GetAttackButton();
	UIManager->PickedButtonIndex = 0;
	BattleMenu->IsChoosingAction = true;
}

void AGameManager::RestartBattleTransitionScreenWidget()
{

}

ABattleManager* AGameManager::GetBattleManager()
{
	return BattleManager;
}

AActor* AGameManager::GetPlayerBattleSpawn()
{
	return PlayerBattleSpawn;
}

TArray<AActor*> AGameManager::GetEnemyBattleSpawns()
{
	return EnemyBattleSpawns;
}




