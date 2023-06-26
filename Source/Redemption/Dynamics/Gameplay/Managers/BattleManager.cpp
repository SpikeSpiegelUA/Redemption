// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\CombatEnemyNPCAIController.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ABattleManager::ABattleManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABattleManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABattleManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Rotate camera to enemy or to a the start position. Only is used in battle mode to follow attacking enemy or select enemy for an attack
	if (CanTurnBehindPlayerCameraToEnemy && IsValid(SelectedEnemy) && IsValid(BehindPlayerCamera)) {
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(BehindPlayerCamera->GetActorLocation(), SelectedEnemy->GetActorLocation());
		BehindPlayerCamera->SetActorRotation(FMath::RInterpTo(BehindPlayerCamera->GetActorRotation(), Rotation, DeltaTime, 7));
		if (BehindPlayerCamera->GetActorRotation().Equals(Rotation))
			CanTurnBehindPlayerCameraToEnemy = false;
	}
	if (CanTurnBehindPlayerCameraToStartPosition && IsValid(BehindPlayerCamera)) {
		FRotator Rotation = FRotator(-16.113443, 180.000000, 0.000000);
		BehindPlayerCamera->SetActorRotation(FMath::RInterpTo(BehindPlayerCamera->GetActorRotation(), Rotation, DeltaTime, 7));
		if (BehindPlayerCamera->GetActorRotation().Equals(Rotation))
			CanTurnBehindPlayerCameraToStartPosition = false;
	}

}

void ABattleManager::SelectNewEnemy(ACombatEnemyNPC* Target, int Index)
{
	if (IsValid(SelectedEnemy) && IsValid(Target)) {
		if (SelectedEnemy->GetEnemyHealthBarWidget())
			SelectedEnemy->GetEnemyHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
		if (Target->GetEnemyHealthBarWidget())
			Target->GetEnemyHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
		SelectedEnemy = Target;
		SelectedEnemyIndex = Index;
		CanTurnBehindPlayerCameraToEnemy = true;
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if(IsValid(PlayerCharacter))
			PlayerCharacter->GetBattleMenuWidget()->SetEnemyName(Target->GetCharacterName());
	}
}

void ABattleManager::TurnChange()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	//Pass turn to next random enemy

	if (IsValid(PlayerCharacter) && PlayerCharacter->CurrentHP > 0) {
		if (EnemyTurnQueue.Num() > 0) {
			ACombatEnemyNPCAIController* AIController{};
			if (ActorNumberOfTheCurrentTurn >= 0) {
				AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController());
				if(IsValid(AIController))
				AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
				BattleEnemies[ActorNumberOfTheCurrentTurn]->SetActorRotation(FRotator(0, 0, 0));
			}
			if (IsValid(SelectedEnemy))
				SelectedEnemy->GetEnemyHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			int NextActor = EnemyTurnQueue[0];
			SelectedEnemy = BattleEnemies[NextActor];
			CanTurnBehindPlayerCameraToEnemy = true;
			EnemyTurnQueue.Remove(NextActor);
			ActorNumberOfTheCurrentTurn = NextActor;
			AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[NextActor]->GetController());
			if (IsValid(AIController))
				if (AIController->GetBlackboardComponent()) {
					AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", true);
				}
		}
		//If EnemyQueue is empty, check if there are alive enemies. If yes, then enable battle UI and continue the battle, if not, show results of the battle.
		else {
			ACombatEnemyNPCAIController* AIController{};
			bool AreAliveEnemies = false;
			for (int i = 0; i < BattleEnemies.Num(); i++)
				if (BattleEnemies[i]->HP > 0)
					AreAliveEnemies = true;
			if (AreAliveEnemies) {
				if (ActorNumberOfTheCurrentTurn >= 0) {
					AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController());
					if(IsValid(AIController))
					AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
					BattleEnemies[ActorNumberOfTheCurrentTurn]->SetActorRotation(FRotator(0, 0, 0));
				}
				TArray<AActor*> UIManagerActors;
				AUIManager* UIManager = nullptr;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUIManager::StaticClass(), UIManagerActors);
				if (UIManagerActors.Num() > 0)
					UIManager = Cast<AUIManager>(UIManagerActors[0]);
				ActorNumberOfTheCurrentTurn = -1;
				UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget();
				CanTurnBehindPlayerCameraToEnemy = false;
				CanTurnBehindPlayerCameraToStartPosition = true;
				if (IsValid(BattleMenu) && IsValid(UIManager)) {
					BattleMenu->IsChoosingAction = true;
					BattleMenu->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
					UIManager->PickedButton = BattleMenu->GetAttackButton();
					BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				//Check Effects array in PlayerCharacter. If CurrentDuration>=Duration, delete the effect
				for (int i = 0; i < PlayerCharacter->Effects.Num(); i++) {
					AEffect*& Effect = PlayerCharacter->Effects[i];
					if (IsValid(Effect)) {
						Effect->CurrentDuration++;
						if (Effect->CurrentDuration >= Effect->GetDuration()) 
							PlayerCharacter->Effects.RemoveAt(i);
					}
				}
				//Set animation variables
				UPlayerCharacterAnimInstance* PlayerCharacterAnimInstance = nullptr;
				if (IsValid(PlayerCharacter))
					if (IsValid(PlayerCharacter->GetMesh()))
						if (IsValid(PlayerCharacter->GetMesh()->GetAnimInstance()))
							PlayerCharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(PlayerCharacter->GetMesh()->GetAnimInstance());
				if (IsValid(PlayerCharacterAnimInstance))
					if (PlayerCharacterAnimInstance->GetIsBlocking())
						PlayerCharacterAnimInstance->SetIsBlocking(false);
			}
			else {
				if (ActorNumberOfTheCurrentTurn >= 0) {
					AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController());
					if(IsValid(AIController))
					AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
					BattleEnemies[ActorNumberOfTheCurrentTurn]->SetActorRotation(FRotator(0, 0, 0));
				}
				//Show results screen
				ActorNumberOfTheCurrentTurn = -1;
				PlayerCharacter->GetBattleMenuWidget()->RemoveFromParent();
				PlayerCharacter->GetBattleResultsScreenWidget()->AddToViewport();
				PlayerCharacter->Effects.Empty();
				for (int i = 0; i < BattleEnemies.Num(); i++)
					TotalGoldReward += BattleEnemies[i]->GoldReward;
				GetWorld()->GetTimerManager().SetTimer(ShowExperienceTextTimerHandle, this, &ABattleManager::ShowExperienceText, 1, false);
				GetWorld()->GetTimerManager().SetTimer(ShowGoldTextTimerHandle, this, &ABattleManager::ShowGoldText, 3, false);
				FTimerDelegate SetAmountOfGoldTimerDelegate = FTimerDelegate::CreateUObject(this, &ABattleManager::SetAmountOfGoldText, TotalGoldReward);
				GetWorldTimerManager().SetTimer(SetAmountOfGoldTextTimerHandle, SetAmountOfGoldTimerDelegate, 4, false);
				GetWorld()->GetTimerManager().SetTimer(ShowContinueButtonTimerHandle, this, &ABattleManager::ShowContinueButton, 5, false);
				//Background Music set
				PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponent->Play(0.0f);
				PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponent->SetPaused(true);
				PlayerCharacter->GetAudioManager()->DungeonBattleResultsBackgroundMusicAudioComponent->Play(0.0f);
				PlayerCharacter->GetAudioManager()->DungeonBattleResultsBackgroundMusicAudioComponent->SetPaused(false);
			}
		}
	}
	//Player's death logic
	else if (IsValid(PlayerCharacter) && PlayerCharacter->CurrentHP <= 0) {
		ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController());
		if (IsValid(AIController))
			AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
		PlayerCharacter->GetBattleMenuWidget()->RemoveFromParent(); 
		PlayerCharacter->GetDeathMenuWidget()->AddToViewport();
		PlayerCharacter->GetAudioManager()->DeathMenuBackgroundMusicAudioComponent->Play(0.0f);
		PlayerCharacter->GetAudioManager()->DeathMenuBackgroundMusicAudioComponent->SetPaused(false);
		PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponent->SetPaused(true);
		APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		if (IsValid(PlayerController))
			PlayerController->SetPause(true);
	}
}

void ABattleManager::PlayerTurnController()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		PlayerCharacter->GetBattleMenuWidget()->IsChoosingAction = true;
		//Delete dead enemies and fill the queue
		for (int i = 0; i < BattleEnemies.Num(); i++)
			if (BattleEnemies[i]->HP <= 0) {
				TotalGoldReward += BattleEnemies[i]->GoldReward;
				BattleEnemies.RemoveAt(i);
			}
		for (int i = 0; i < BattleEnemies.Num(); i++)
			EnemyTurnQueue.Add(i);
		RandomizeEnemyQueue(EnemyTurnQueue);
		TurnChange();
	}
}

void ABattleManager::RandomizeEnemyQueue(TArray<int> &Array)
{
	if (Array.Num() > 0)
	{
		int32 LastIndex = Array.Num() - 1;
		for (int32 i = 0; i < LastIndex; i++)
		{
			int32 Index = FMath::RandRange(0, LastIndex);
			if (i != Index)
			{
				Array.Swap(i, Index);
			}
		}
	}
}

void ABattleManager::ShowExperienceText()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	PlayerCharacter->GetBattleResultsScreenWidget()->GetExperienceTextBlock()->SetVisibility(ESlateVisibility::Visible);
}

void ABattleManager::ShowGoldText()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	PlayerCharacter->GetBattleResultsScreenWidget()->GetGoldTextBlock()->SetVisibility(ESlateVisibility::Visible);
}

void ABattleManager::SetAmountOfGoldText(int Value)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	PlayerCharacter->GetBattleResultsScreenWidget()->SetAmountOfGoldTextBlock(FText::FromString(FString::FromInt(Value)));
}

void ABattleManager::ShowContinueButton()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	PlayerCharacter->GetBattleResultsScreenWidget()->GetContinueButton()->SetVisibility(ESlateVisibility::Visible);
}

void ABattleManager::SetCanTurnBehindPlayerCameraToEnemy(bool Value)
{
	CanTurnBehindPlayerCameraToEnemy = Value;
}

void ABattleManager::SetCanTurnBehindPlayerCameraToStartPosition(bool Value)
{
	CanTurnBehindPlayerCameraToStartPosition = Value;
}

void ABattleManager::AddEnemyTurnQueue(int Value)
{
	EnemyTurnQueue.Add(Value);
}

void ABattleManager::SetActorNumberOfTheCurrentTurn(uint8 Value)
{
	ActorNumberOfTheCurrentTurn = Value;
}

ACameraActor* ABattleManager::GetBehindPlayerCamera() const
{
	return BehindPlayerCamera;
}

uint8 ABattleManager::GetActorNumberOfTheCurrentTurn() const
{
	return ActorNumberOfTheCurrentTurn;
}

TArray<int> ABattleManager::GetEnemyTurnQueue() const
{
	return EnemyTurnQueue;
}

