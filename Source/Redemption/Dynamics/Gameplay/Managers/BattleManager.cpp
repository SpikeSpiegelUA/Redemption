// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "..\Dynamics\Gameplay\Skills and Effects\TurnStartDamageEffect.h"
#include "..\Characters\AI Controllers\Combat\CombatEnemyNPCAIController.h"
#include "..\UI\Menus\BattleMenu.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "UIManagerWorldSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "..\Miscellaneous\ArrayActions.h"
#include "..\Miscellaneous\BattleActions.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\UI\HUD\FloatingManaBarWidget.h"
#include "..\Characters\Animation\Combat\CombatAlliesAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Redemption/Characters/AI Controllers/Combat/CombatAlliesAIController.h"
#include "D:/UE_5.1/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"

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
		BehindPlayerCamera->SetActorRotation(FMath::RInterpTo(BehindPlayerCamera->GetActorRotation(), Rotation, DeltaTime, 8.f));
		if (BehindPlayerCamera->GetActorRotation().Equals(Rotation))
			CanTurnBehindPlayerCameraToTarget = false;
	}
	else if (CanTurnBehindPlayerCameraToStartPosition && IsValid(BehindPlayerCamera)) {
		FRotator Rotation = FRotator(-16.113443, 180.000000, 0.000000);
		BehindPlayerCamera->SetActorRotation(FMath::RInterpTo(BehindPlayerCamera->GetActorRotation(), Rotation, DeltaTime, 8.f));
		if (BehindPlayerCamera->GetActorRotation().Equals(Rotation))
			CanTurnBehindPlayerCameraToStartPosition = false;
	}

}

void ABattleManager::SelectNewTarget(const ACombatNPC* const Target, int8 Index)
{
	if (IsValid(Target)) {
		//if (SelectedCombatNPC->GetFloatingHealthBarWidget())
		//	SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
		//if (Target->GetFloatingHealthBarWidget())
		//	Target->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
		SelectedCombatNPC = const_cast<ACombatNPC*>(Target);
		SelectedCombatNPCIndex = Index;
		CanTurnBehindPlayerCameraToTarget = true;
		if (CurrentTurnCombatNPCIndex >= 0 && CurrentTurnCombatNPCIndex < BattleAlliesPlayer.Num())
			BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Target = const_cast<ACombatNPC*>(Target);
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if(IsValid(PlayerCharacter))
			PlayerCharacter->GetBattleMenuWidget()->SetTargetName(FText::FromName(Target->GetCharacterName()));
	}
}

void ABattleManager::SelectNewTargetCrosshairLogic(const TArray<ACombatNPC*>& TargetsForSelection, int8 NewIndex, int8 CurrentIndex, const std::string_view Direction)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		//Value corresponds to the Range value: 1 - Single, 2 - Neighbors, 3 - Everybody.
		uint8 RangeValue{};
		if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell || PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem) {
			if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell) {
				if (IsValid(PlayerCharacter->GetSpellBattleMenuWidget()->GetCreatedSpell())) {
					switch (PlayerCharacter->GetSpellBattleMenuWidget()->GetCreatedSpell()->GetSpellRange()) {
						case ESpellRange::SINGLE:
							RangeValue = 1;
							break;
						case ESpellRange::NEIGHBORS:
							RangeValue = 2;
							break;
						case ESpellRange::EVERYONE:
							RangeValue = 3;
							break;
						}
				}
			}
			else if (PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem) {
				if (IsValid(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem())) {
					switch (PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem()->GetItemRange()) {
						case EItemRange::SINGLE:
							RangeValue = 1;
							break;
						case EItemRange::NEIGHBORS:
							RangeValue = 2;
							break;
						case EItemRange::EVERYONE:
							RangeValue = 3;
							break;
						}
				}
			}
			if (RangeValue == 1) {
				TargetsForSelection[NewIndex]->GetCrosshairWidgetComponent()->SetVisibility(true);
				TargetsForSelection[NewIndex]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
				TargetsForSelection[CurrentIndex]->GetCrosshairWidgetComponent()->SetVisibility(false);
				TargetsForSelection[CurrentIndex]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			}
			else if (RangeValue == 2) {
				TargetsForSelection[NewIndex]->GetCrosshairWidgetComponent()->SetVisibility(true);
				TargetsForSelection[NewIndex]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
				if (NewIndex == 0) {
					TargetsForSelection[CurrentIndex]->GetCrosshairWidgetComponent()->SetVisibility(false);
					TargetsForSelection[CurrentIndex]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
					if (NewIndex + 1 < TargetsForSelection.Num()) {
						TargetsForSelection[NewIndex + 1]->GetCrosshairWidgetComponent()->SetVisibility(true);
						TargetsForSelection[NewIndex + 1]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
					}
				}
				else if (NewIndex == TargetsForSelection.Num() - 1) {
					TargetsForSelection[CurrentIndex]->GetCrosshairWidgetComponent()->SetVisibility(false);
					TargetsForSelection[CurrentIndex]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
					if (NewIndex - 1 >= 0) {
						TargetsForSelection[NewIndex - 1]->GetCrosshairWidgetComponent()->SetVisibility(true);
						TargetsForSelection[NewIndex - 1]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
					}
				}
				else {
					TargetsForSelection[CurrentIndex]->GetCrosshairWidgetComponent()->SetVisibility(true);
					TargetsForSelection[CurrentIndex]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
				}
				if (Direction == "Left") {
					if (CurrentIndex - 1 >= 0) {
						TargetsForSelection[CurrentIndex - 1]->GetCrosshairWidgetComponent()->SetVisibility(false);
						TargetsForSelection[CurrentIndex - 1]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
					}
					if (CurrentIndex + 1 < TargetsForSelection.Num()) {
						TargetsForSelection[CurrentIndex + 1]->GetCrosshairWidgetComponent()->SetVisibility(true);
						TargetsForSelection[CurrentIndex + 1]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
					}
					if (CurrentIndex + 2 < TargetsForSelection.Num()) {
						TargetsForSelection[CurrentIndex + 2]->GetCrosshairWidgetComponent()->SetVisibility(true);
						TargetsForSelection[CurrentIndex + 2]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
					}
				}
				else if(Direction == "Right") {
					if (CurrentIndex + 1 < TargetsForSelection.Num()) {
						TargetsForSelection[CurrentIndex + 1]->GetCrosshairWidgetComponent()->SetVisibility(false);
						TargetsForSelection[CurrentIndex + 1]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
					}
					if (CurrentIndex - 1 >= 0) {
						TargetsForSelection[CurrentIndex - 1]->GetCrosshairWidgetComponent()->SetVisibility(true);
						TargetsForSelection[CurrentIndex - 1]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
					}
					if (CurrentIndex - 2 >= 0) {
						TargetsForSelection[CurrentIndex - 2]->GetCrosshairWidgetComponent()->SetVisibility(true);
						TargetsForSelection[CurrentIndex - 2]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
					}
				}
			}
			else if (RangeValue == 3) {

			}
		}
		else {
			TargetsForSelection[NewIndex]->GetCrosshairWidgetComponent()->SetVisibility(true);
			TargetsForSelection[NewIndex]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
			TargetsForSelection[CurrentIndex]->GetCrosshairWidgetComponent()->SetVisibility(false);
			TargetsForSelection[CurrentIndex]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABattleManager::TurnChange()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	//Pass turn to next random enemy
	if (IsValid(CombatPlayerCharacter) && CombatPlayerCharacter->CurrentHP > 0 && IsValid(PlayerCharacter)) {
		if (EnemyTurnQueue.Num() > 0) {
			if (CurrentTurnCombatNPCIndex >= 0) {
				if(ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[CurrentTurnCombatNPCIndex]->GetController()); IsValid(AIController))
					AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
				BattleEnemies[CurrentTurnCombatNPCIndex]->SetActorRotation(FRotator(0, 0, 0));
			}
			/*for (int i = BattleAlliesPlayer.Num() - 1; i >= 0; i--)
				if (BattleAlliesPlayer[i]->GetCurrentHP() <= 0) 
					BattleAlliesPlayer.RemoveAt(i);*/
			if(IsValid(SelectedCombatNPC))
				SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
			if (IsValid(Cast<ACombatAllies>(SelectedCombatNPC)))
				Cast<ACombatAllies>(SelectedCombatNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Hidden);
			int8 NextActor = EnemyTurnQueue[0];
			SelectedCombatNPC = BattleEnemies[NextActor];
			CurrentTurnCombatNPCIndex = NextActor;
			//Check if skip the turn.
			bool ContinueTurn = true;
			for (int16 i = BattleEnemies[NextActor]->Effects.Num() - 1; i >= 0; i--)
				if(IsValid(BattleEnemies[NextActor]->Effects[i]))
					if (BattleEnemies[NextActor]->Effects[i]->GetEffectType() == EEffectType::TURNSKIP)
						if (IsValid(BattleEnemies[NextActor]->GetMesh())) {
							BattleEnemies[NextActor]->Effects[i]->CurrentDuration += 1;
							if (BattleEnemies[NextActor]->Effects[i]->CurrentDuration >= BattleEnemies[NextActor]->Effects[i]->GetDuration() + 1) {
								BattleEnemies[NextActor]->Effects[i]->ConditionalBeginDestroy();
								BattleEnemies[NextActor]->Effects.RemoveAt(i);
								continue;
							}
							BattleEnemies[NextActor]->GetMesh()->bPauseAnims = true;
							ContinueTurn = false;
						}
			if (ContinueTurn) {
				if (IsValid(BattleEnemies[NextActor]->GetMesh()))
					if(BattleEnemies[NextActor]->GetMesh()->bPauseAnims)
						BattleEnemies[NextActor]->GetMesh()->bPauseAnims = false;
				//If the next actor has a TurnStartDamage effect, then use get hit and delay AI activation by 2 seconds. If the next actor is dizzy, set CanTargetEveryone to true.
				bool GotHit = false;
				for (AEffect* Effect : BattleEnemies[NextActor]->Effects) {
					if (Effect->GetEffectType() == EEffectType::TURNSTARTDAMAGE) {
						if (ATurnStartDamageEffect* TurnStartDamageEffect = Cast<ATurnStartDamageEffect>(Effect); IsValid(TurnStartDamageEffect)) {
							BattleEnemies[NextActor]->Execute_GetHit(PlayerCharacter->GetBattleManager()->BattleEnemies[NextActor],
								BattleActions::CalculateAttackValueAfterEffects(Effect->GetEffectStat(), BattleEnemies[NextActor]),
								ElementsActions::FindContainedElements(TurnStartDamageEffect->GetSpellElements()), false);
							GotHit = true;
						}
					}
					else if (Effect->GetEffectType() == EEffectType::DIZZINESS) {
						if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[NextActor]->GetController()); IsValid(AIController))
							if (IsValid(AIController->GetBlackboardComponent()))
								AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("CanTargetEveryone", true);
					}
				}
				//Destroy effects, which CurrentDuration is >= Duration.
				for (int16 i = BattleEnemies[NextActor]->Effects.Num() - 1; i >= 0; i--) {
					if (IsValid(BattleEnemies[NextActor]->Effects[i])) {
						BattleEnemies[NextActor]->Effects[i]->CurrentDuration += 1;
						if (BattleEnemies[NextActor]->Effects[i]->CurrentDuration >= BattleEnemies[NextActor]->Effects[i]->GetDuration() + 1) {
							if (BattleEnemies[NextActor]->Effects[i]->GetEffectType() == EEffectType::DIZZINESS) {
								if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[NextActor]->GetController()); IsValid(AIController))
									if (IsValid(AIController->GetBlackboardComponent())) {
										AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("CanTargetEveryone", false);
										if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->DizzyEmitterComponent))
											BattleEnemies[NextActor]->DizzyEmitterComponent->DestroyComponent();
									}
							}
							else if (BattleEnemies[NextActor]->Effects[i]->GetEffectType() == EEffectType::TURNSTARTDAMAGE) {
								if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->FlamesEmitterComponent))
									BattleEnemies[NextActor]->FlamesEmitterComponent->DestroyComponent();
							}
							BattleEnemies[NextActor]->Effects[i]->ConditionalBeginDestroy();
							BattleEnemies[NextActor]->Effects.RemoveAt(i);
						}
					}
				}
				if (GotHit) {
					FTimerHandle EnableTurnAIControllerTimerHandle{};
					GetWorld()->GetTimerManager().SetTimer(EnableTurnAIControllerTimerHandle, this, &ABattleManager::EnableTurnAIController, 2.25f, false);
				}
				else {
					if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[NextActor]->GetController()); IsValid(AIController))
						if (IsValid(AIController->GetBlackboardComponent()))
							AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", true);
				}
			}
			else {
				FTimerHandle SkipTurnTimerHandle{};
				GetWorld()->GetTimerManager().SetTimer(SkipTurnTimerHandle, this, &ABattleManager::SkipEnemyTurnActions, 1.f, false);
			}
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
				if (CurrentTurnCombatNPCIndex >= 0) {
					if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[CurrentTurnCombatNPCIndex]->GetController()); IsValid(AIController))
						AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
					BattleEnemies[CurrentTurnCombatNPCIndex]->SetActorRotation(FRotator(0, 0, 0));
				}
				UUIManagerWorldSubsystem* UIManagerWorldSubsystem = nullptr;
				if (IsValid(GetWorld()))
					UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
				UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget();
				CanTurnBehindPlayerCameraToTarget = false;
				CanTurnBehindPlayerCameraToStartPosition = true;
				/*for (int8 i = BattleAlliesPlayer.Num() - 1; i >= 0; i--)
					if (BattleAlliesPlayer[i]->GetCurrentHP() <= 0)
						BattleAlliesPlayer.RemoveAt(i);*/
				for (int i = BattleAlliesPlayer.Num() - 1; i >= 0; i--) 
					if(BattleAlliesPlayer[i]->GetCurrentHP() > 0)
						AlliesPlayerTurnQueue.Add(i);
				ArrayActions::ShuffleArray<int>(AlliesPlayerTurnQueue);
				if (BattleAlliesPlayer.Num() > 0)
					BehindPlayerCamera->SetActorLocation(Cast<ACombatStartLocation>(BattleAlliesPlayer[AlliesPlayerTurnQueue[0]]->GetStartLocation())->CombatCameraLocation);
				CurrentTurnCombatNPCIndex = AlliesPlayerTurnQueue[0];
				if (UAlliesInfoBars* AlliesInfoBarsWidget = PlayerCharacter->GetAlliesInfoBarsWidget(); IsValid(AlliesInfoBarsWidget)) {
					AlliesInfoBarsWidget->GetAlliesNameBorders()[AlliesPlayerTurnQueue[0]]->SetBrushColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder = AlliesPlayerTurnQueue[0];
				}
				AlliesPlayerTurnQueue.RemoveAt(0);
				//Check, if skip the turn.
				bool ContinueTurn = true;
				for (int i = BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.Num() - 1; i >= 0; i--)
					if(IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]))
						if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetEffectType() == EEffectType::TURNSKIP)
							if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh())) {
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration += 1;
								if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration >= BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetDuration() + 1) {
									BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->ConditionalBeginDestroy();
									BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.RemoveAt(i);
									continue;
								}
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->bPauseAnims = true;
								ContinueTurn = false;
							}
				if (ContinueTurn) {
					//Toggle blocking animation off
					UCombatAlliesAnimInstance* CombatAlliesAnimInstance{};
					if (CurrentTurnCombatNPCIndex >= 0) {
						if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()))
							if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->bPauseAnims)
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->bPauseAnims = false;
						if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]))
							if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()))
								if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->GetAnimInstance()))
									CombatAlliesAnimInstance = Cast<UCombatAlliesAnimInstance>(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->GetAnimInstance());
						if (IsValid(CombatAlliesAnimInstance))
							if (CombatAlliesAnimInstance->GetCombatAlliesIsBlocking())
								CombatAlliesAnimInstance->ToggleCombatAlliesIsBlocking(false);
					}
					//If the next actor has a TurnStartDamage effect, then use get hit and delay AI activation by 2 seconds.
					bool GotHit = false;
					bool IsDizzy = false;
					for (AEffect* Effect : BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects) {
						if (Effect->GetEffectType() == EEffectType::TURNSTARTDAMAGE) {
							if (ATurnStartDamageEffect* TurnStartDamageEffect = Cast<ATurnStartDamageEffect>(Effect); IsValid(TurnStartDamageEffect)) {
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Execute_GetHit(PlayerCharacter->GetBattleManager()->BattleAlliesPlayer[CurrentTurnCombatNPCIndex],
									BattleActions::CalculateAttackValueAfterEffects(Effect->GetEffectStat(), BattleAlliesPlayer[CurrentTurnCombatNPCIndex]),
									ElementsActions::FindContainedElements(TurnStartDamageEffect->GetSpellElements()), false);
								GotHit = true;
							}
						}
						else if (Effect->GetEffectType() == EEffectType::DIZZINESS) {
							IsDizzy = true;
						}
					}
					//Destroy effects, which CurrentDuration is >= Duration.
					for (int i = BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.Num() - 1; i >= 0; i--) {
						if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i])) {
							BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration += 1;
							if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration >= BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetDuration() + 1) {
								if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetEffectType() == EEffectType::DIZZINESS) {
									if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetController()); IsValid(AIController))
										if (IsValid(AIController->GetBlackboardComponent())) {
											AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("CanTargetEveryone", false);
											if(IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->DizzyEmitterComponent))
												BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->DizzyEmitterComponent->DestroyComponent();
										}
								}
								else if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetEffectType() == EEffectType::TURNSTARTDAMAGE) {
									if(IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->FlamesEmitterComponent))
										BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->FlamesEmitterComponent->DestroyComponent();
								}
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->ConditionalBeginDestroy();
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.RemoveAt(i);
							}
						}
					}
					if (GotHit) {
						FTimerHandle GotHitTimerHandle{};
						if (IsDizzy)
							GetWorld()->GetTimerManager().SetTimer(GotHitTimerHandle, this, &ABattleManager::PlayerAllyDizzyActions, 2.25f, false);
						else
							GetWorld()->GetTimerManager().SetTimer(GotHitTimerHandle, this, &ABattleManager::ToPlayerTurnPassInTurnChangeFunction, 2.25f, false);
					}
					else {
						if(IsDizzy)
							PlayerAllyDizzyActions();
						else
							ToPlayerTurnPassInTurnChangeFunction();
					}
				}
				else {
					FTimerHandle SkipTurnTimerHandle{};
					GetWorld()->GetTimerManager().SetTimer(SkipTurnTimerHandle, this, &ABattleManager::SkipAllyTurnActions, 1.f, false);
				}
			}
			else {
				if (CurrentTurnCombatNPCIndex >= 0) {
					if(ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[CurrentTurnCombatNPCIndex]->GetController()); IsValid(AIController))
					AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
					BattleEnemies[CurrentTurnCombatNPCIndex]->SetActorRotation(FRotator(0, 0, 0));
				}
				//Show results screen
				CurrentTurnCombatNPCIndex = -1;
				PlayerCharacter->GetBattleMenuWidget()->RemoveFromParent();
				PlayerCharacter->GetBattleResultsScreenWidget()->AddToViewport();
				GetWorld()->GetTimerManager().SetTimer(ShowExperienceTextTimerHandle, this, &ABattleManager::ShowExperienceText, 1.f, false);
				GetWorld()->GetTimerManager().SetTimer(ShowGoldTextTimerHandle, this, &ABattleManager::ShowGoldText, 3.f, false);
				FTimerDelegate SetAmountOfGoldTimerDelegate = FTimerDelegate::CreateUObject(this, &ABattleManager::SetAmountOfGoldText, PlayerCharacter->Gold);
				GetWorldTimerManager().SetTimer(SetAmountOfGoldTextTimerHandle, SetAmountOfGoldTimerDelegate, 4.f, false);
				GetWorld()->GetTimerManager().SetTimer(ShowContinueButtonTimerHandle, this, &ABattleManager::ShowContinueButton, 5.f, false);
				//Background Music set
				PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->Play(0.0f);
				PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
				PlayerCharacter->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->Play(0.0f);
				PlayerCharacter->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->SetPaused(false);
				for (int8 i = CombatPlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
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
		if(CurrentTurnCombatNPCIndex >= 0)
			if (auto* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[CurrentTurnCombatNPCIndex]->GetController()); IsValid(AIController))
				AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", false);
		PlayerCharacter->GetBattleMenuWidget()->RemoveFromParent(); 
		PlayerCharacter->GetDeathMenuWidget()->AddToViewport();
		PlayerCharacter->GetAudioManager()->GetDeathMenuBackgroundMusicAudioComponent()->Play(0.0f);
		PlayerCharacter->GetAudioManager()->GetDeathMenuBackgroundMusicAudioComponent()->SetPaused(false);
		PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
		APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		for (int8 i = CombatPlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
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
			for (int8 i = 0; i < BattleEnemies.Num(); i++)
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
				BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->SetActorRotation(FRotator(0, 180, 0));
				if (IsValid(SelectedCombatNPC))
					SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
				if (IsValid(Cast<ACombatAllies>(SelectedCombatNPC)))
					Cast<ACombatAllies>(SelectedCombatNPC)->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(ESlateVisibility::Hidden);
				int8 NextActor = AlliesPlayerTurnQueue[0];
				SelectedCombatNPC = BattleAlliesPlayer[NextActor];
				CurrentTurnCombatNPCIndex = NextActor;
				BehindPlayerCamera->SetActorLocation(Cast<ACombatStartLocation>(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetStartLocation())->CombatCameraLocation);
				if (UAlliesInfoBars* AlliesInfoBarsWidget = PlayerCharacter->GetAlliesInfoBarsWidget(); IsValid(AlliesInfoBarsWidget)) {
					if (AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder < AlliesInfoBarsWidget->GetAlliesNameBorders().Num())
						AlliesInfoBarsWidget->GetAlliesNameBorders()[AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder]->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
					AlliesInfoBarsWidget->GetAlliesNameBorders()[CurrentTurnCombatNPCIndex]->SetBrushColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder = CurrentTurnCombatNPCIndex;
				}
				//CanTurnBehindPlayerCameraToTarget = true;
				AlliesPlayerTurnQueue.Remove(NextActor);
				UUIManagerWorldSubsystem* UIManagerWorldSubsystem = nullptr;
				if (IsValid(GetWorld()))
					UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
				UBattleMenu* BattleMenu = PlayerCharacter->GetBattleMenuWidget();
				CanTurnBehindPlayerCameraToTarget = false;
				CanTurnBehindPlayerCameraToStartPosition = true;
				//Check if skip the turn.
				bool ContinueTurn = true;
				for (int i = BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.Num() - 1; i >= 0; i--)
					if(IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]))
						if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetEffectType() == EEffectType::TURNSKIP)
							if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh())) {
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration += 1;
								UE_LOG(LogTemp, Warning, TEXT("Error!!!"));
								if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration >= BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetDuration() + 1) {
									BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->ConditionalBeginDestroy();
									BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.RemoveAt(i);
									continue;
								}
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->bPauseAnims = true;
								ContinueTurn = false;
							}
				if (ContinueTurn) {
					//Set animation variables
					UCombatAlliesAnimInstance* CombatAlliesAnimInstance{};
					if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]))
						if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()))
							if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->GetAnimInstance()))
								CombatAlliesAnimInstance = Cast<UCombatAlliesAnimInstance>(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->GetAnimInstance());
					if (IsValid(CombatAlliesAnimInstance))
						if (CombatAlliesAnimInstance->GetCombatAlliesIsBlocking())
							CombatAlliesAnimInstance->ToggleCombatAlliesIsBlocking(false);
					//If the next actor has a TurnStartDamage effect, then use get hit and delay AI activation by 2 seconds.
					bool GotHit = false;
					bool IsDizzy = false;
					for (AEffect* Effect : BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects) {
						if (Effect->GetEffectType() == EEffectType::TURNSTARTDAMAGE) {
							if (ATurnStartDamageEffect* TurnStartDamageEffect = Cast<ATurnStartDamageEffect>(Effect); IsValid(TurnStartDamageEffect)) {
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Execute_GetHit(PlayerCharacter->GetBattleManager()->BattleAlliesPlayer[CurrentTurnCombatNPCIndex],
									BattleActions::CalculateAttackValueAfterEffects(Effect->GetEffectStat(), BattleAlliesPlayer[CurrentTurnCombatNPCIndex]),
									ElementsActions::FindContainedElements(TurnStartDamageEffect->GetSpellElements()), false);
								GotHit = true;
							}
						}
						else if (Effect->GetEffectType() == EEffectType::DIZZINESS) {
							IsDizzy = true;
						}
					}
					//Destroy effects, which CurrentDuration is >= Duration.
					for (int i = BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.Num() - 1; i >= 0; i--) {
						if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i])) {
							BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration += 1;
							UE_LOG(LogTemp, Warning, TEXT("The current duration is: %d"), BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration);
							UE_LOG(LogTemp, Warning, TEXT("The overall duration is: %d"), BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetDuration() + 1);
							if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration >= BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetDuration() + 1) {
								if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetEffectType() == EEffectType::DIZZINESS) {
									if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetController()); IsValid(AIController))
										if (IsValid(AIController->GetBlackboardComponent())) {
											AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("CanTargetEveryone", false);
											if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->DizzyEmitterComponent))
												BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->DizzyEmitterComponent->DestroyComponent();
										}
								}
								else if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetEffectType() == EEffectType::TURNSTARTDAMAGE) {
									if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->FlamesEmitterComponent))
										BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->FlamesEmitterComponent->DestroyComponent();
								}
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->ConditionalBeginDestroy();
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.RemoveAt(i);
							}
						}
					}
					if (GotHit) {
						FTimerHandle GotHitTimerHandle{};
						if (IsDizzy)
							GetWorld()->GetTimerManager().SetTimer(GotHitTimerHandle, this, &ABattleManager::PlayerAllyDizzyActions, 2.25f, false);
						else
							GetWorld()->GetTimerManager().SetTimer(GotHitTimerHandle, this, &ABattleManager::ToPlayerTurnPassInTurnChangeFunction, 2.25f, false);
					}
					else {
						if (IsDizzy)
							PlayerAllyDizzyActions();
						else
							ToPlayerTurnPassInTurnChangeFunction();
					}
					/*if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleEnemies[NextActor]->GetController()); IsValid(AIController))
						if (IsValid(AIController->GetBlackboardComponent()))
							AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", true);*/
				}
				else {
					FTimerHandle SkipTurnTimerHandle{};
					GetWorld()->GetTimerManager().SetTimer(SkipTurnTimerHandle, this, &ABattleManager::SkipAllyTurnActions, 1.f, false);
				}
			}
			else {
				//Show results screen
				CurrentTurnCombatNPCIndex = -1;
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
				if (IsValid(SelectedCombatNPC))
					SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
				for (int16 i = CombatPlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
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
			if(CurrentTurnCombatNPCIndex >= 0)
				BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->SetActorRotation(FRotator(0, 180, 0));
			CurrentTurnCombatNPCIndex = -1;
			for (int8 i = BattleEnemies.Num() - 1; i >= 0; i--)
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

void ABattleManager::EnableTurnAIController()
{
	if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(SelectedCombatNPC->GetController()); IsValid(AIController))
		if (IsValid(AIController->GetBlackboardComponent()))
			AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("Actor's Turn", true);
}

void ABattleManager::SkipEnemyTurnActions()
{
	TurnChange();
}

void ABattleManager::SkipAllyTurnActions()
{
	PlayerTurnController();
}

void ABattleManager::ToPlayerTurnPassInTurnChangeFunction()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) 
		if (UBattleMenu* BattleMenu = Cast<UBattleMenu>(PlayerCharacter->GetBattleMenuWidget()); IsValid(BattleMenu)){
			if (IsValid(BattleMenu)) {
				if (!BattleMenu->IsInViewport())
					BattleMenu->AddToViewport();
				BattleMenu->IsChoosingAction = true;
				BattleMenu->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
				if(IsValid(GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>()))
					GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>()->PickedButton = BattleMenu->GetAttackButton();
				BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			if (CurrentTurnCombatNPCIndex >= 0) {
				//Check Effects array in a next turn's character. If CurrentDuration>=Duration, add index to special array and then delete in separate loop
				//1 is subtracted from the duration right after the turn, so we need to set the duration to desired number of turns + 1.
				for (int i = BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.Num() - 1; i >= 0; i--) {
					if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i])) {
						BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration += 1;
						if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration >= BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetDuration() + 1) {
							BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->ConditionalBeginDestroy();
							BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.RemoveAt(i);
						}
					}
				}
				PlayerCharacter->GetSkillBattleMenuWidget()->ResetSkillsScrollBox();
				for (TSubclassOf<ASpell> SpellClass : BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetAvailableSkills())
					PlayerCharacter->GetSkillBattleMenuWidget()->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
			}
	}
}

void ABattleManager::PlayerAllyDizzyActions()
{
	TArray<ACombatNPC*> ActorsToTarget{};
	for (ACombatNPC* Actor : BattleAlliesPlayer)
		if (Actor != BattleAlliesPlayer[CurrentTurnCombatNPCIndex])
			ActorsToTarget.Add(Actor);
	for (ACombatNPC* Actor : BattleEnemies)
			ActorsToTarget.Add(Actor);

	//Get Targets total targeting chance.
	int16 SumOfTargetingChances{};
	for (int8 i = 0; i < ActorsToTarget.Num(); i++)
		SumOfTargetingChances += ActorsToTarget[i]->TargetingChance;

	//Get random number from zero to total chance. Then set sum to 0 and gradually add chances to sum again, but check if random number is less or equal than sum. If yes, than
	//set TargetActor and quit from the loop.
	int16 RandomTargetingChance = FMath::RandRange(0, SumOfTargetingChances);
	SumOfTargetingChances = 0;

	for (int8 i = 0; i < ActorsToTarget.Num(); i++) {
		SumOfTargetingChances += ActorsToTarget[i]->TargetingChance;
		if (RandomTargetingChance <= SumOfTargetingChances) {
			BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Target = ActorsToTarget[i];
			SelectedCombatNPC = ActorsToTarget[i];
			break;
		}
	}

	if (ACombatAllies* CurrentNPCCombatAllies = Cast<ACombatAllies>(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]); IsValid(CurrentNPCCombatAllies)) {
		CanTurnBehindPlayerCameraToTarget = true;
		CanTurnBehindPlayerCameraToStartPosition = false;
		CurrentNPCCombatAllies->StartMovingToEnemy();
	}
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

