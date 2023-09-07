// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\Combat\CombatEnemyNPCAIController.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\UIManager.h"
#include <Kismet/KismetMathLibrary.h>
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ArrayActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\FloatingManaBarWidget.h"
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
	if (CanTurnBehindPlayerCameraToTarget && IsValid(SelectedCombatNPC) && IsValid(BehindPlayerCamera)) {
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(BehindPlayerCamera->GetActorLocation(), SelectedCombatNPC->GetActorLocation());
		BehindPlayerCamera->SetActorRotation(FMath::RInterpTo(BehindPlayerCamera->GetActorRotation(), Rotation, DeltaTime, 7));
		if (BehindPlayerCamera->GetActorRotation().Equals(Rotation))
			CanTurnBehindPlayerCameraToTarget = false;
	}
	if (CanTurnBehindPlayerCameraToStartPosition && IsValid(BehindPlayerCamera)) {
		FRotator Rotation = FRotator(-16.113443, 180.000000, 0.000000);
		BehindPlayerCamera->SetActorRotation(FMath::RInterpTo(BehindPlayerCamera->GetActorRotation(), Rotation, DeltaTime, 7));
		if (BehindPlayerCamera->GetActorRotation().Equals(Rotation))
			CanTurnBehindPlayerCameraToStartPosition = false;
	}

}

void ABattleManager::SelectNewTarget(ACombatNPC* const& Target, int Index)
{
	if (IsValid(Target)) {
		if (SelectedCombatNPC->GetFloatingHealthBarWidget())
			SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
		if (Target->GetFloatingHealthBarWidget())
			Target->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
		SelectedCombatNPC = Target;
		SelectedCombatNPCIndex = Index;
		CanTurnBehindPlayerCameraToTarget = true;
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if(IsValid(PlayerCharacter))
			PlayerCharacter->GetBattleMenuWidget()->SetTargetName(FText::FromName(Target->GetCharacterName()));
	}
}

void ABattleManager::TurnChange()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	//Pass turn to next random enemy/ally

	if (IsValid(PlayerCharacter) && PlayerCharacter->CurrentHP > 0) {
		if (EnemyTurnQueue.Num() > 0) {
			if (ActorNumberOfTheCurrentTurn >= 0) {
				if(ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController()); IsValid(AIController))
					AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
				BattleEnemies[ActorNumberOfTheCurrentTurn]->SetActorRotation(FRotator(0, 0, 0));
			}
			SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			if (IsValid(Cast<ACombatAllies>(SelectedCombatNPC)))
				Cast<ACombatAllies>(SelectedCombatNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Hidden);
			int NextActor = EnemyTurnQueue[0];
			SelectedCombatNPC = BattleEnemies[NextActor];
			CanTurnBehindPlayerCameraToTarget = true;
			EnemyTurnQueue.Remove(NextActor);
			ActorNumberOfTheCurrentTurn = NextActor;
			if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[NextActor]->GetController()); IsValid(AIController))
				if (IsValid(AIController->GetBlackboardComponent()))
					AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", true);
		}
		//If EnemyQueue is empty, check if there are alive enemies. If yes, then enable battle UI and continue the battle, if not, show results of the battle.
		else {
			bool AreAliveEnemies = false;
			for (int i = 0; i < BattleEnemies.Num(); i++)
				if (BattleEnemies[i]->GetHP() > 0)
					AreAliveEnemies = true;
			if (AreAliveEnemies) {
				if (ActorNumberOfTheCurrentTurn >= 0) {
					if(ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController()); IsValid(AIController))
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
				CanTurnBehindPlayerCameraToTarget = false;
				CanTurnBehindPlayerCameraToStartPosition = true;
				if (IsValid(BattleMenu) && IsValid(UIManager)) {
					BattleMenu->IsChoosingAction = true;
					BattleMenu->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
					UIManager->PickedButton = BattleMenu->GetAttackButton();
					BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				//Check Effects array in PlayerCharacter. If CurrentDuration>=Duration, add index to special array and then delete in separate loop
				for (int i = PlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
					if (IsValid(PlayerCharacter->Effects[i])) {
						PlayerCharacter->Effects[i]->CurrentDuration += 1;
						if (PlayerCharacter->Effects[i]->CurrentDuration >= PlayerCharacter->Effects[i]->GetDuration()) {
							PlayerCharacter->Effects[i]->ConditionalBeginDestroy();
							PlayerCharacter->Effects.RemoveAt(i);
						}
					}
				}
				//Set animation variables
				UPlayerCharacterAnimInstance* PlayerCharacterAnimInstance = nullptr;
				if (IsValid(PlayerCharacter))
					if (IsValid(PlayerCharacter->GetMesh()))
						if (IsValid(PlayerCharacter->GetMesh()->GetAnimInstance()))
							PlayerCharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(PlayerCharacter->GetMesh()->GetAnimInstance());
				if (IsValid(PlayerCharacterAnimInstance))
					if (PlayerCharacterAnimInstance->GetPlayerIsBlock())
						PlayerCharacterAnimInstance->SetPlayerIsBlock(false);
			}
			else {
				if (ActorNumberOfTheCurrentTurn >= 0) {
					if(ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController()); IsValid(AIController))
					AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
					BattleEnemies[ActorNumberOfTheCurrentTurn]->SetActorRotation(FRotator(0, 0, 0));
				}
				//Show results screen
				ActorNumberOfTheCurrentTurn = -1;
				PlayerCharacter->GetBattleMenuWidget()->RemoveFromParent();
				PlayerCharacter->GetBattleResultsScreenWidget()->AddToViewport();
				PlayerCharacter->Effects.Empty();
				for (int i = 0; i < BattleEnemies.Num(); i++)
					TotalGoldReward += BattleEnemies[i]->GetGoldReward();
				GetWorld()->GetTimerManager().SetTimer(ShowExperienceTextTimerHandle, this, &ABattleManager::ShowExperienceText, 1, false);
				GetWorld()->GetTimerManager().SetTimer(ShowGoldTextTimerHandle, this, &ABattleManager::ShowGoldText, 3, false);
				FTimerDelegate SetAmountOfGoldTimerDelegate = FTimerDelegate::CreateUObject(this, &ABattleManager::SetAmountOfGoldText, TotalGoldReward);
				GetWorldTimerManager().SetTimer(SetAmountOfGoldTextTimerHandle, SetAmountOfGoldTimerDelegate, 4, false);
				GetWorld()->GetTimerManager().SetTimer(ShowContinueButtonTimerHandle, this, &ABattleManager::ShowContinueButton, 5, false);
				//Background Music set
				PlayerCharacter->GetAudioManager()->GetDungeonCombatBackgroundMusicAudioComponent()->Play(0.0f);
				PlayerCharacter->GetAudioManager()->GetDungeonCombatBackgroundMusicAudioComponent()->SetPaused(true);
				PlayerCharacter->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->Play(0.0f);
				PlayerCharacter->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->SetPaused(false);
				for (int i = PlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
					if (IsValid(PlayerCharacter->Effects[i])) {
						PlayerCharacter->Effects[i]->ConditionalBeginDestroy();
						PlayerCharacter->Effects.RemoveAt(i);
					}
				}
			}
		}
	}
	//Player's death logic
	else if (IsValid(PlayerCharacter) && PlayerCharacter->CurrentHP <= 0) {
		if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController()); IsValid(AIController))
			AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
		PlayerCharacter->GetBattleMenuWidget()->RemoveFromParent(); 
		PlayerCharacter->GetDeathMenuWidget()->AddToViewport();
		PlayerCharacter->GetAudioManager()->GetDeathMenuBackgroundMusicAudioComponent()->Play(0.0f);
		PlayerCharacter->GetAudioManager()->GetDeathMenuBackgroundMusicAudioComponent()->SetPaused(false);
		PlayerCharacter->GetAudioManager()->GetDungeonCombatBackgroundMusicAudioComponent()->SetPaused(true);
		APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		for (int i = PlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
			if (IsValid(PlayerCharacter->Effects[i])) {
				PlayerCharacter->Effects[i]->ConditionalBeginDestroy();
				PlayerCharacter->Effects.RemoveAt(i);
			}
		}
		if (IsValid(PlayerController))
			PlayerController->SetPause(true);
	}
}

void ABattleManager::SetTimerForPlayerTurnController()
{
	GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, this, &ABattleManager::PlayerTurnController, 2.1f, false);
}

void ABattleManager::PlayerTurnController()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		PlayerCharacter->GetBattleMenuWidget()->IsChoosingAction = true;
		//Delete dead enemies and fill the queue
		for (int i = 0; i < BattleEnemies.Num(); i++)
			if (BattleEnemies[i]->GetHP() <= 0) 
				TotalGoldReward += BattleEnemies[i]->GetGoldReward();
			else
				EnemyTurnQueue.Add(i);
		ArrayActions::ShuffleArray<int>(EnemyTurnQueue);
		TurnChange();
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

void ABattleManager::SetCanTurnBehindPlayerCameraToTarget(bool Value)
{
	CanTurnBehindPlayerCameraToTarget = Value;
}

void ABattleManager::SetCanTurnBehindPlayerCameraToStartPosition(bool Value)
{
	CanTurnBehindPlayerCameraToStartPosition = Value;
}

void ABattleManager::SetActorNumberOfTheCurrentTurn(uint8 Value)
{
	ActorNumberOfTheCurrentTurn = Value;
}

void ABattleManager::SetBehindPlayerCameraLocation(FVector NewLocation)
{
	BehindPlayerCamera->SetActorLocation(NewLocation);
}

ACameraActor* ABattleManager::GetBehindPlayerCamera() const
{
	return BehindPlayerCamera;
}

FTimerHandle ABattleManager::GetPlayerTurnControllerTimerHandle() const
{
	return PlayerTurnControllerTimerHandle;
}

TSubclassOf<ACombatFloatingInformationActor> ABattleManager::GetCombatFloatingInformationActorClass() const
{
	return CombatFloatingInformationActorClass;
}

uint8 ABattleManager::GetActorNumberOfTheCurrentTurn() const
{
	return ActorNumberOfTheCurrentTurn;
}

