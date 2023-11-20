// Fill out your copyright notice in the Description page of Project Settings.


#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\Combat\CombatEnemyNPCAIController.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\BattleMenu.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "UIManagerWorldSubsystem.h"
#include <Kismet/KismetMathLibrary.h>
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ArrayActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\FloatingManaBarWidget.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\Combat\CombatAlliesAnimInstance.h"
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

void ABattleManager::SelectNewTarget(const ACombatNPC* const Target, int Index)
{
	if (IsValid(Target)) {
		if (SelectedCombatNPC->GetFloatingHealthBarWidget())
			SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
		if (Target->GetFloatingHealthBarWidget())
			Target->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
		SelectedCombatNPC = const_cast<ACombatNPC*>(Target);
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
	//Pass turn to next random enemy
	if (IsValid(CombatPlayerCharacter) && CombatPlayerCharacter->CurrentHP > 0) {
		if (EnemyTurnQueue.Num() > 0) {
			if (ActorNumberOfTheCurrentTurn >= 0) {
				if(ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController()); IsValid(AIController))
					AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
				BattleEnemies[ActorNumberOfTheCurrentTurn]->SetActorRotation(FRotator(0, 0, 0));
			}
			/*for (int i = BattleAlliesPlayer.Num() - 1; i >= 0; i--)
				if (BattleAlliesPlayer[i]->GetCurrentHP() <= 0) 
					BattleAlliesPlayer.RemoveAt(i);*/
			if(IsValid(SelectedCombatNPC))
				SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			if (IsValid(Cast<ACombatAllies>(SelectedCombatNPC)))
				Cast<ACombatAllies>(SelectedCombatNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Hidden);
				int NextActor = EnemyTurnQueue[0];
				SelectedCombatNPC = BattleEnemies[NextActor];
				ActorNumberOfTheCurrentTurn = NextActor;
				if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[NextActor]->GetController()); IsValid(AIController))
					if (IsValid(AIController->GetBlackboardComponent()))
						AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", true);
				EnemyTurnQueue.RemoveAt(0);
			CanTurnBehindPlayerCameraToTarget = true;
		}
		//If EnemyQueue is empty, check if there are alive enemies. If yes, then enable battle UI and continue the battle, if not, show results of the battle.
		else {
			bool AreAliveEnemies = false;
			for (int i = 0; i < BattleEnemies.Num(); i++)
				if (BattleEnemies[i]->GetCurrentHP() > 0) {
					AreAliveEnemies = true;
					break;
				}
			if (AreAliveEnemies) {
				if (ActorNumberOfTheCurrentTurn >= 0) {
					if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController()); IsValid(AIController))
						AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
					BattleEnemies[ActorNumberOfTheCurrentTurn]->SetActorRotation(FRotator(0, 0, 0));
				}
				UUIManagerWorldSubsystem* UIManagerWorldSubsystem = nullptr;
				if (IsValid(GetWorld()))
					UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
				ActorNumberOfTheCurrentTurn = -1;
				UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget();
				CanTurnBehindPlayerCameraToTarget = false;
				CanTurnBehindPlayerCameraToStartPosition = true;
				/*for (int8 i = BattleAlliesPlayer.Num() - 1; i >= 0; i--)
					if (BattleAlliesPlayer[i]->GetCurrentHP() <= 0)
						BattleAlliesPlayer.RemoveAt(i);*/
				for (int8 i = BattleAlliesPlayer.Num() - 1; i >= 0; i--) 
					if(BattleAlliesPlayer[i]->GetCurrentHP() > 0)
						AlliesPlayerTurnQueue.Add(i);
				ArrayActions::ShuffleArray<int>(AlliesPlayerTurnQueue);
				if (BattleAlliesPlayer.Num() > 0)
					BehindPlayerCamera->SetActorLocation(Cast<ACombatStartLocation>(BattleAlliesPlayer[AlliesPlayerTurnQueue[0]]->GetStartLocation())->CombatCameraLocation);
				CurrentTurnAllyPlayerIndex = AlliesPlayerTurnQueue[0];
				if (UAlliesInfoBars* AlliesInfoBarsWidget = PlayerCharacter->GetAlliesInfoBarsWidget(); IsValid(AlliesInfoBarsWidget)) {
					AlliesInfoBarsWidget->GetAlliesNameBorders()[AlliesPlayerTurnQueue[0]]->SetBrushColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder = AlliesPlayerTurnQueue[0];
				}
				AlliesPlayerTurnQueue.RemoveAt(0);
				if (IsValid(BattleMenu) && IsValid(UIManagerWorldSubsystem)) {
					BattleMenu->AddToViewport();
					BattleMenu->IsChoosingAction = true;
					BattleMenu->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
					UIManagerWorldSubsystem->PickedButton = BattleMenu->GetAttackButton();
					BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				//Toggle blocking animation off
				UCombatAlliesAnimInstance* CombatAlliesAnimInstance{};
				if (CurrentTurnAllyPlayerIndex >= 0 ) {
					if (IsValid(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]))
						if (IsValid(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->GetMesh()))
							if (IsValid(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->GetMesh()->GetAnimInstance()))
								CombatAlliesAnimInstance = Cast<UCombatAlliesAnimInstance>(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->GetMesh()->GetAnimInstance());
					//Check Effects array in a next turn's character. If CurrentDuration>=Duration, add index to special array and then delete in separate loop
					//1 is subtracted from the duration right after the turn, so we need to set the duration to desired number of turns + 1.
					for (int i = BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects.Num() - 1; i >= 0; i--) {
						if (IsValid(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects[i])) {
							BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects[i]->CurrentDuration += 1;
							if (BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects[i]->CurrentDuration >= BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects[i]->GetDuration() + 1) {
								BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects[i]->ConditionalBeginDestroy();
								BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects.RemoveAt(i);
							}
						}
					}
					PlayerCharacter->GetSkillBattleMenuWidget()->ResetSkillsScrollBox();
					for (TSubclassOf<ASpell> SpellClass : BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->GetAvailableSkills())
						PlayerCharacter->GetSkillBattleMenuWidget()->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
				}
				if (IsValid(CombatAlliesAnimInstance))
					if (CombatAlliesAnimInstance->GetCombatAlliesIsBlocking())
						CombatAlliesAnimInstance->ToggleCombatAlliesIsBlocking(false);
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
				GetWorld()->GetTimerManager().SetTimer(ShowExperienceTextTimerHandle, this, &ABattleManager::ShowExperienceText, 1, false);
				GetWorld()->GetTimerManager().SetTimer(ShowGoldTextTimerHandle, this, &ABattleManager::ShowGoldText, 3, false);
				FTimerDelegate SetAmountOfGoldTimerDelegate = FTimerDelegate::CreateUObject(this, &ABattleManager::SetAmountOfGoldText, PlayerCharacter->Gold);
				GetWorldTimerManager().SetTimer(SetAmountOfGoldTextTimerHandle, SetAmountOfGoldTimerDelegate, 4, false);
				GetWorld()->GetTimerManager().SetTimer(ShowContinueButtonTimerHandle, this, &ABattleManager::ShowContinueButton, 5, false);
				//Background Music set
				PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->Play(0.0f);
				PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
				PlayerCharacter->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->Play(0.0f);
				PlayerCharacter->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->SetPaused(false);
				for (int i = CombatPlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
					if (IsValid(CombatPlayerCharacter->Effects[i])) {
						CombatPlayerCharacter->Effects[i]->ConditionalBeginDestroy();
						CombatPlayerCharacter->Effects.RemoveAt(i);
					}
				}
			}
		}

	}
	//Player's death logic
	else if (IsValid(CombatPlayerCharacter) && CombatPlayerCharacter->CurrentHP <= 0) {
		if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController()); IsValid(AIController))
			AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
		PlayerCharacter->GetBattleMenuWidget()->RemoveFromParent(); 
		PlayerCharacter->GetDeathMenuWidget()->AddToViewport();
		PlayerCharacter->GetAudioManager()->GetDeathMenuBackgroundMusicAudioComponent()->Play(0.0f);
		PlayerCharacter->GetAudioManager()->GetDeathMenuBackgroundMusicAudioComponent()->SetPaused(false);
		PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
		APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		for (int i = CombatPlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
			if (IsValid(CombatPlayerCharacter->Effects[i])) {
				CombatPlayerCharacter->Effects[i]->ConditionalBeginDestroy();
				CombatPlayerCharacter->Effects.RemoveAt(i);
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
		IsSelectingAllyAsTarget = false;
		if (AlliesPlayerTurnQueue.Num() > 0) {
			bool AreAliveEnemies = false;
			for (int i = 0; i < BattleEnemies.Num(); i++)
				if (BattleEnemies[i]->GetCurrentHP() > 0) {
					AreAliveEnemies = true;
					break;
				}
			if (AreAliveEnemies) {
				/*	if (ActorNumberOfTheCurrentTurn >= 0) {
						if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[ActorNumberOfTheCurrentTurn]->GetController()); IsValid(AIController))
							AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
						BattleEnemies[ActorNumberOfTheCurrentTurn]->SetActorRotation(FRotator(0, 0, 0));
					}*/
				BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->SetActorRotation(FRotator(0, 180, 0));
				if (IsValid(SelectedCombatNPC))
					SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
				if (IsValid(Cast<ACombatAllies>(SelectedCombatNPC)))
					Cast<ACombatAllies>(SelectedCombatNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Hidden);
				int NextActor = AlliesPlayerTurnQueue[0];
				SelectedCombatNPC = BattleAlliesPlayer[NextActor];
				CurrentTurnAllyPlayerIndex = NextActor;
				BehindPlayerCamera->SetActorLocation(Cast<ACombatStartLocation>(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->GetStartLocation())->CombatCameraLocation);
				if (UAlliesInfoBars* AlliesInfoBarsWidget = PlayerCharacter->GetAlliesInfoBarsWidget(); IsValid(AlliesInfoBarsWidget)) {
					if (AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder < AlliesInfoBarsWidget->GetAlliesNameBorders().Num())
						AlliesInfoBarsWidget->GetAlliesNameBorders()[AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder]->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
					AlliesInfoBarsWidget->GetAlliesNameBorders()[CurrentTurnAllyPlayerIndex]->SetBrushColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder = CurrentTurnAllyPlayerIndex;
				}
				//CanTurnBehindPlayerCameraToTarget = true;
				AlliesPlayerTurnQueue.Remove(NextActor);
				ActorNumberOfTheCurrentTurn = NextActor;
				UUIManagerWorldSubsystem* UIManagerWorldSubsystem = nullptr;
				if (IsValid(GetWorld()))
					UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
				ActorNumberOfTheCurrentTurn = -1;
				UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget();
				CanTurnBehindPlayerCameraToTarget = false;
				CanTurnBehindPlayerCameraToStartPosition = true;
				if (IsValid(BattleMenu) && IsValid(UIManagerWorldSubsystem)) {
					BattleMenu->AddToViewport();
					BattleMenu->IsChoosingAction = true;
					BattleMenu->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
					UIManagerWorldSubsystem->PickedButton = BattleMenu->GetAttackButton();
					BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
				}
				//Check Effects array in a next character. If CurrentDuration>=Duration, add index to special array and then delete in separate loop
				//1 is subtracted from the duration right after the turn, so we need to set the duration to desired number of turns + 1.
				for (int i = BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects.Num() - 1; i >= 0; i--) {
					if (IsValid(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects[i])) {
						BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects[i]->CurrentDuration += 1;
						if (BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects[i]->CurrentDuration >= BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects[i]->GetDuration() + 1) {
							BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects[i]->ConditionalBeginDestroy();
							BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->Effects.RemoveAt(i);
						}
					}
				}
				//Set animation variables
				UCombatAlliesAnimInstance* CombatAlliesAnimInstance{};
				if (IsValid(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]))
					if (IsValid(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->GetMesh()))
						if (IsValid(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->GetMesh()->GetAnimInstance()))
							CombatAlliesAnimInstance = Cast<UCombatAlliesAnimInstance>(BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->GetMesh()->GetAnimInstance());
				if (IsValid(CombatAlliesAnimInstance))
					if (CombatAlliesAnimInstance->GetCombatAlliesIsBlocking())
						CombatAlliesAnimInstance->ToggleCombatAlliesIsBlocking(false);
				PlayerCharacter->GetSkillBattleMenuWidget()->ResetSkillsScrollBox();
				for (TSubclassOf<ASpell> SpellClass : BattleAlliesPlayer[CurrentTurnAllyPlayerIndex]->GetAvailableSkills())
					PlayerCharacter->GetSkillBattleMenuWidget()->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
				/*if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[NextActor]->GetController()); IsValid(AIController))
					if (IsValid(AIController->GetBlackboardComponent()))
						AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", true);*/
			}
			else {
				//Show results screen
				ActorNumberOfTheCurrentTurn = -1;
				PlayerCharacter->GetBattleMenuWidget()->RemoveFromParent();
				PlayerCharacter->GetBattleResultsScreenWidget()->AddToViewport();
				GetWorld()->GetTimerManager().SetTimer(ShowExperienceTextTimerHandle, this, &ABattleManager::ShowExperienceText, 1, false);
				GetWorld()->GetTimerManager().SetTimer(ShowGoldTextTimerHandle, this, &ABattleManager::ShowGoldText, 3, false);
				FTimerDelegate SetAmountOfGoldTimerDelegate = FTimerDelegate::CreateUObject(this, &ABattleManager::SetAmountOfGoldText, PlayerCharacter->Gold);
				GetWorldTimerManager().SetTimer(SetAmountOfGoldTextTimerHandle, SetAmountOfGoldTimerDelegate, 4, false);
				GetWorld()->GetTimerManager().SetTimer(ShowContinueButtonTimerHandle, this, &ABattleManager::ShowContinueButton, 5, false);
				//Background Music set
				PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->Play(0.0f);
				PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
				PlayerCharacter->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->Play(0.0f);
				PlayerCharacter->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->SetPaused(false);
				for (int i = CombatPlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
					if (IsValid(CombatPlayerCharacter->Effects[i])) {
						CombatPlayerCharacter->Effects[i]->ConditionalBeginDestroy();
						CombatPlayerCharacter->Effects.RemoveAt(i);
					}
				}
			}
		}
		else {
			//Delete dead enemies and fill the queue
			/*for (int i = BattleEnemies.Num() - 1; i >= 0; i--)
				if (BattleEnemies[i]->GetCurrentHP() <= 0) {
					PlayerCharacter->Gold += BattleEnemies[i]->GetGoldReward();
					BattleEnemies.RemoveAt(i);
				}*/
			if (UAlliesInfoBars* AlliesInfoBarsWidget = PlayerCharacter->GetAlliesInfoBarsWidget(); IsValid(AlliesInfoBarsWidget)) 
				if (AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder < AlliesInfoBarsWidget->GetAlliesNameBorders().Num())
					AlliesInfoBarsWidget->GetAlliesNameBorders()[AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder]->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
			for (int i = BattleEnemies.Num() - 1; i >= 0; i--)
				if(BattleEnemies[i]->GetCurrentHP() > 0)
					EnemyTurnQueue.Add(i);
			ArrayActions::ShuffleArray<int>(EnemyTurnQueue);
			TurnChange();
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

void ABattleManager::SetBehindPlayerCameraLocation(FVector& NewLocation)
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

