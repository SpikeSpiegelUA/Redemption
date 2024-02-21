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
#include "Redemption/Dynamics/World/Items/RestorationItem.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"

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
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	verify(PlayerCharacter);
	if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		RedemptionGameModeBase->SetBattleManager(this);
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
	else if (CanTurnBehindPlayerCameraToSpellObject && IsValid(BehindPlayerCamera) && IsValid(CurrentlyUsedSpellObject)) {
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(BehindPlayerCamera->GetActorLocation(), CurrentlyUsedSpellObject->GetActorLocation());
		BehindPlayerCamera->SetActorRotation(FMath::RInterpTo(BehindPlayerCamera->GetActorRotation(), Rotation, DeltaTime, 8.f));
		if (BehindPlayerCamera->GetActorRotation().Equals(Rotation))
			CanTurnBehindPlayerCameraToSpellObject = false;
	}

}

void ABattleManager::SelectNewTarget(const ACombatNPC* const Target, int8 Index)
{
	if (IsValid(Target)) {
		SelectedCombatNPC = const_cast<ACombatNPC*>(Target);
		SelectedCombatNPCIndex = Index;
		CanTurnBehindPlayerCameraToTarget = true;
		if (CurrentTurnCombatNPCIndex >= 0 && CurrentTurnCombatNPCIndex < BattleAlliesPlayer.Num())
			BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Target = const_cast<ACombatNPC*>(Target);
		if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
			UIManagerWorldSubsystem->BattleMenuWidget->SetTargetName(FText::FromName(Target->GetCharacterName()));
	}
}

void ABattleManager::SelectNewTargetCrosshairLogic(const TArray<ACombatNPC*>& TargetsForSelection, int8 NewIndex, int8 CurrentIndex, const std::string_view Direction)
{
	if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		//Value corresponds to the Range value: 1 - Single, 2 - Neighbors.
		uint8 RangeValue{};
		//Health or Mana.
		FString TypeOfBar = "Health";
		if (UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithSpell || UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithItem) {
			if (UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithSpell) {
				if (ASpell* CreatedSpell = UIManagerWorldSubsystem->SpellBattleMenuWidget->GetCreatedSpell(); IsValid(CreatedSpell)) {
					switch (UIManagerWorldSubsystem->SpellBattleMenuWidget->GetCreatedSpell()->GetSpellRange()) {
						case ESpellRange::SINGLE:
							RangeValue = 1;
							break;
						case ESpellRange::NEIGHBORS:
							RangeValue = 2;
							break;
					}
					if (ARestorationSpell* RestorationSpell = Cast<ARestorationSpell>(CreatedSpell); IsValid(RestorationSpell))
						if (RestorationSpell->GetTypeOfRestoration() == ESpellRestorationType::MANA)
							TypeOfBar = "Mana";
				}
			}
			else if (UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithItem) {
				if (AGameItem* Item = UIManagerWorldSubsystem->InventoryMenuWidget->GetPickedItem(); IsValid(Item)) {
					switch (Item->GetItemRange()) {
						case EItemRange::SINGLE:
							RangeValue = 1;
							break;
						case EItemRange::NEIGHBORS:
							RangeValue = 2;
							break;
					}
					if (auto* RestorationItem = Cast<ARestorationItem>(Item); IsValid(RestorationItem))
						if (RestorationItem->GetTypeOfRestoration() == EItemRestorationType::MANA)
							TypeOfBar = "Mana";
				}
			}
			if (RangeValue == 1) {
				SelectNewTargetCrosshairActions(TargetsForSelection, NewIndex, TypeOfBar, true);
				SelectNewTargetCrosshairActions(TargetsForSelection, CurrentIndex, TypeOfBar, false);
			}
			else if (RangeValue == 2) {
				if (TargetsForSelection.Num() >= 3) {
					SelectNewTargetCrosshairActions(TargetsForSelection, NewIndex, TypeOfBar, true);
					if (Direction == "Left") {
						if (CurrentIndex - 1 >= 0) 
							SelectNewTargetCrosshairActions(TargetsForSelection, CurrentIndex - 1, TypeOfBar, false);
						if (CurrentIndex + 1 < TargetsForSelection.Num())
							SelectNewTargetCrosshairActions(TargetsForSelection, CurrentIndex + 1, TypeOfBar, true);
						if (CurrentIndex + 2 < TargetsForSelection.Num())
							SelectNewTargetCrosshairActions(TargetsForSelection, CurrentIndex + 2, TypeOfBar, true);
					}
					else if (Direction == "Right") {
						if (CurrentIndex + 1 < TargetsForSelection.Num())
							SelectNewTargetCrosshairActions(TargetsForSelection, CurrentIndex + 1, TypeOfBar, false);
						if (CurrentIndex - 1 >= 0) 
							SelectNewTargetCrosshairActions(TargetsForSelection, CurrentIndex - 1, TypeOfBar, true);
						if (CurrentIndex - 2 >= 0) 
							SelectNewTargetCrosshairActions(TargetsForSelection, CurrentIndex - 2, TypeOfBar, true);
					}
					if (NewIndex == 0) {
						SelectNewTargetCrosshairActions(TargetsForSelection, CurrentIndex, TypeOfBar, false);
						if (NewIndex + 1 < TargetsForSelection.Num())
							SelectNewTargetCrosshairActions(TargetsForSelection, NewIndex + 1, TypeOfBar, true);
					}
					else if (NewIndex == TargetsForSelection.Num() - 1) {
						SelectNewTargetCrosshairActions(TargetsForSelection, CurrentIndex, TypeOfBar, false);
						if (NewIndex - 1 >= 0)
							SelectNewTargetCrosshairActions(TargetsForSelection, NewIndex - 1, TypeOfBar, true);
					}
					else
						SelectNewTargetCrosshairActions(TargetsForSelection, CurrentIndex, TypeOfBar, true);
				}
				else if(TargetsForSelection.Num() == 2) {
					SelectNewTargetCrosshairActions(TargetsForSelection, 0, TypeOfBar, true);
					SelectNewTargetCrosshairActions(TargetsForSelection, 1, TypeOfBar, true);
				}
				else if (TargetsForSelection.Num() == 1) {
					SelectNewTargetCrosshairActions(TargetsForSelection, 0, TypeOfBar, true);
				}
			}
		}
		else {
			if (!UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithRange)
				TargetsForSelection[NewIndex]->GetCrosshairWidgetComponent()->SetVisibility(true);
			TargetsForSelection[NewIndex]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
			TargetsForSelection[CurrentIndex]->GetCrosshairWidgetComponent()->SetVisibility(false);
			TargetsForSelection[CurrentIndex]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ABattleManager::SelectNewTargetCrosshairActions(const TArray<ACombatNPC*>& TargetsForSelection, int8 Index, const FString& TypeOfBar, const bool WhetherToShow)
{
	ESlateVisibility SlateVisibility{};
	if (WhetherToShow)
		SlateVisibility = ESlateVisibility::Visible;
	else
		SlateVisibility = ESlateVisibility::Hidden;
	TargetsForSelection[Index]->GetCrosshairWidgetComponent()->SetVisibility(WhetherToShow);
	if (TypeOfBar == "Health")
		TargetsForSelection[Index]->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(SlateVisibility);
	else if (TypeOfBar == "Mana")
		if (ACombatAllies* CombatAlliesNPC = Cast<ACombatAllies>(TargetsForSelection[Index]); IsValid(CombatAlliesNPC))
			CombatAlliesNPC->GetFloatingManaBarWidget()->GetManaBar()->SetVisibility(SlateVisibility);
}

void ABattleManager::TurnChange()
{
	//Pass turn to next random enemy
	if (IsValid(CombatPlayerCharacter) && CombatPlayerCharacter->CurrentHP > 0 && IsValid(PlayerCharacter)) {
		if (EnemyTurnQueue.Num() > 0) {
			if (CurrentTurnCombatNPCIndex >= 0) {
				BattleEnemies[CurrentTurnCombatNPCIndex]->SetActorRotation(FRotator(0, 0, 0));
			}
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
							if(const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
								BattleEnemies[NextActor]->Execute_GetHit(RedemptionGameModeBase->GetBattleManager()->BattleEnemies[NextActor],
									BattleActions::CalculateAttackValueAfterEffects(Effect->GetEffectStat(), BattleEnemies[NextActor]),
									ElementsActions::FindContainedElements(TurnStartDamageEffect->GetSpellElements()), EPhysicalType::NONE, false);
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
						if (BattleEnemies[NextActor]->Effects[i]->GetEffectType() != EEffectType::TURNSKIP) {
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
				}
				if (GotHit) {
					if (IsValid(CombatPlayerCharacter) && CombatPlayerCharacter->CurrentHP <= 0) {
						PlayerDeathLogic();
					}
					else {
						FTimerHandle EnableTurnAIControllerTimerHandle{};
						GetWorld()->GetTimerManager().SetTimer(EnableTurnAIControllerTimerHandle, this, &ABattleManager::EnableTurnAIController, 2.25f, false);
					}
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
			for (int8 i = 0; i < BattleEnemies.Num(); i++)
				if (BattleEnemies[i]->GetCurrentHP() > 0) {
					AreAliveEnemies = true;
					break;
				}
			if (AreAliveEnemies) {
				if (CurrentTurnCombatNPCIndex >= 0) {
					BattleEnemies[CurrentTurnCombatNPCIndex]->SetActorRotation(FRotator(0, 0, 0));
				}
				UUIManagerWorldSubsystem* UIManagerWorldSubsystem = nullptr;
				if (IsValid(GetWorld()))
					UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
				UBattleMenu* BattleMenu = UIManagerWorldSubsystem->BattleMenuWidget;
				CanTurnBehindPlayerCameraToTarget = false;
				CanTurnBehindPlayerCameraToStartPosition = true;
				/*for (int8 i = BattleAlliesPlayer.Num() - 1; i >= 0; i--)
					if (BattleAlliesPlayer[i]->GetCurrentHP() <= 0)
						BattleAlliesPlayer.RemoveAt(i);*/
				for (int8 i = BattleAlliesPlayer.Num() - 1; i >= 0; i--) 
					if(BattleAlliesPlayer[i]->GetCurrentHP() > 0)
						AlliesPlayerTurnQueue.Add(i);
				ArrayActions::ShuffleArray<uint8>(AlliesPlayerTurnQueue);
				if (BattleAlliesPlayer.Num() > 0)
					BehindPlayerCamera->SetActorLocation(Cast<ACombatStartLocation>(BattleAlliesPlayer[AlliesPlayerTurnQueue[0]]->GetStartLocation())->CombatCameraLocation);
				CurrentTurnCombatNPCIndex = AlliesPlayerTurnQueue[0];
				if (UAlliesInfoBars* AlliesInfoBarsWidget = UIManagerWorldSubsystem->AlliesInfoBarsWidget; IsValid(AlliesInfoBarsWidget)) {
					AlliesInfoBarsWidget->GetAlliesNameBorders()[AlliesPlayerTurnQueue[0]]->SetBrushColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder = AlliesPlayerTurnQueue[0];
				}
				AlliesPlayerTurnQueue.RemoveAt(0);
				//Check, if skip the turn.
				bool ContinueTurn = true;
				for (int16 i = BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.Num() - 1; i >= 0; i--)
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
							if(const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
								if (ATurnStartDamageEffect* TurnStartDamageEffect = Cast<ATurnStartDamageEffect>(Effect); IsValid(TurnStartDamageEffect)) {
									BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Execute_GetHit(RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer[CurrentTurnCombatNPCIndex],
										BattleActions::CalculateAttackValueAfterEffects(Effect->GetEffectStat(), BattleAlliesPlayer[CurrentTurnCombatNPCIndex]),
										ElementsActions::FindContainedElements(TurnStartDamageEffect->GetSpellElements()), EPhysicalType::NONE, false);
									GotHit = true;
								}
						}
						else if (Effect->GetEffectType() == EEffectType::DIZZINESS) {
							IsDizzy = true;
						}
					}
					if (GotHit) {
						if (IsValid(CombatPlayerCharacter) && CombatPlayerCharacter->CurrentHP <= 0) {
							PlayerDeathLogic();
						}
						else {
							FTimerHandle GotHitTimerHandle{};
							if (IsDizzy)
								GetWorld()->GetTimerManager().SetTimer(GotHitTimerHandle, this, &ABattleManager::PlayerAllyDizzyActions, 1.9f, false);
							else
								GetWorld()->GetTimerManager().SetTimer(GotHitTimerHandle, this, &ABattleManager::ToPlayerTurnPassInTurnChangeFunction, 1.9f, false);
						}
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
					BattleEnemies[CurrentTurnCombatNPCIndex]->SetActorRotation(FRotator(0, 0, 0));
				}
				EndBattle();
			}
		}

	}
	//Player's death logic
	else if (IsValid(CombatPlayerCharacter) && CombatPlayerCharacter->CurrentHP <= 0) {
		PlayerDeathLogic();
	}
}

void ABattleManager::SetTimerForPlayerTurnController()
{
	GetWorld()->GetTimerManager().SetTimer(PlayerTurnControllerTimerHandle, this, &ABattleManager::PlayerTurnController, 2.1f, false);
}

void ABattleManager::PlayerTurnController()
{
	if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingAction = true;
		UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingSpell = false;
		UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithSpell = false;
		UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToAttack = false;
		UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithRange = false;
		UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToTalk = false;
		UIManagerWorldSubsystem->BattleMenuWidget->IsPreparingToViewInfo = false;
		UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingItem = false;
		UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithItem = false;
		UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithMelee = false;
		UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingSkill = false;
		UIManagerWorldSubsystem->BattleMenuWidget->IsChoosingLearnedSpell = false;
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
				if (UAlliesInfoBars* AlliesInfoBarsWidget = UIManagerWorldSubsystem->AlliesInfoBarsWidget; IsValid(AlliesInfoBarsWidget)) {
					if (AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder < AlliesInfoBarsWidget->GetAlliesNameBorders().Num())
						AlliesInfoBarsWidget->GetAlliesNameBorders()[AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder]->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
					AlliesInfoBarsWidget->GetAlliesNameBorders()[CurrentTurnCombatNPCIndex]->SetBrushColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
					AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder = CurrentTurnCombatNPCIndex;
				}
				//CanTurnBehindPlayerCameraToTarget = true;
				AlliesPlayerTurnQueue.Remove(NextActor);
				UBattleMenu* BattleMenu = UIManagerWorldSubsystem->BattleMenuWidget;
				CanTurnBehindPlayerCameraToTarget = false;
				CanTurnBehindPlayerCameraToStartPosition = true;
				//Check if skip the turn.
				bool ContinueTurn = true;
				for (int16 i = BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.Num() - 1; i >= 0; i--)
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
					//Set animation variables
					UCombatAlliesAnimInstance* CombatAlliesAnimInstance{};
					if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]))
						if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh())) {
							if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->GetAnimInstance()))
								CombatAlliesAnimInstance = Cast<UCombatAlliesAnimInstance>(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->GetAnimInstance());
							if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->bPauseAnims)
								BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetMesh()->bPauseAnims = false;
						}
					if (IsValid(CombatAlliesAnimInstance))
						if (CombatAlliesAnimInstance->GetCombatAlliesIsBlocking())
							CombatAlliesAnimInstance->ToggleCombatAlliesIsBlocking(false);
					//If the next actor has a TurnStartDamage effect, then use get hit and delay AI activation by 2 seconds.
					bool GotHit = false;
					bool IsDizzy = false;
					for (AEffect* Effect : BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects) {
						if (Effect->GetEffectType() == EEffectType::TURNSTARTDAMAGE) {
							if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
								if (ATurnStartDamageEffect* TurnStartDamageEffect = Cast<ATurnStartDamageEffect>(Effect); IsValid(TurnStartDamageEffect)) {
									BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Execute_GetHit(RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer[CurrentTurnCombatNPCIndex],
										BattleActions::CalculateAttackValueAfterEffects(Effect->GetEffectStat(), BattleAlliesPlayer[CurrentTurnCombatNPCIndex]),
										ElementsActions::FindContainedElements(TurnStartDamageEffect->GetSpellElements()), EPhysicalType::NONE, false);
									GotHit = true;
								}
						}
						else if (Effect->GetEffectType() == EEffectType::DIZZINESS) {
							IsDizzy = true;
						}
					}
					if (GotHit) {
						if (IsValid(CombatPlayerCharacter) && CombatPlayerCharacter->CurrentHP <= 0) {
							PlayerDeathLogic();
						}
						else {
							FTimerHandle GotHitTimerHandle{};
							if (IsDizzy)
								GetWorld()->GetTimerManager().SetTimer(GotHitTimerHandle, this, &ABattleManager::PlayerAllyDizzyActions, 1.9f, false);
							else
								GetWorld()->GetTimerManager().SetTimer(GotHitTimerHandle, this, &ABattleManager::ToPlayerTurnPassInTurnChangeFunction, 1.9f, false);
						}
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
			else
				EndBattle();
		}
		else {
			if (UAlliesInfoBars* AlliesInfoBarsWidget = UIManagerWorldSubsystem->AlliesInfoBarsWidget; IsValid(AlliesInfoBarsWidget))
				if (AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder < AlliesInfoBarsWidget->GetAlliesNameBorders().Num())
					AlliesInfoBarsWidget->GetAlliesNameBorders()[AlliesInfoBarsWidget->IndexOfCurrentTurnCharacterNameBorder]->SetBrushColor(FLinearColor(0.3f, 0.3f, 0.3f, 1.f));
			if(CurrentTurnCombatNPCIndex >= 0)
				BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->SetActorRotation(FRotator(0, 180, 0));
			CurrentTurnCombatNPCIndex = -1;
			for (int8 i = BattleEnemies.Num() - 1; i >= 0; i--)
				if(BattleEnemies[i]->GetCurrentHP() > 0)
					EnemyTurnQueue.Add(i);
			ArrayActions::ShuffleArray<uint8>(EnemyTurnQueue);
			TurnChange();
		}
	}
}

void ABattleManager::PlayerAlliesEffectsDurationLogic(const TArray<uint8>& PassedAlliesPlayerTurnQueue)
{
	if (IsValid(PlayerCharacter)) {
		for (int8 Index : PassedAlliesPlayerTurnQueue) {
			//Check if skip the turn.
			bool ContinueTurn = true;
			for (int16 i = BattleAlliesPlayer[Index]->Effects.Num() - 1; i >= 0; i--)
				if (IsValid(BattleAlliesPlayer[Index]->Effects[i]))
					if (BattleAlliesPlayer[Index]->Effects[i]->GetEffectType() == EEffectType::TURNSKIP)
						if (IsValid(BattleAlliesPlayer[Index]->GetMesh())) {
							BattleAlliesPlayer[Index]->Effects[i]->CurrentDuration += 1;
							if (BattleAlliesPlayer[Index]->Effects[i]->CurrentDuration >= BattleAlliesPlayer[Index]->Effects[i]->GetDuration() + 1) {
								BattleAlliesPlayer[Index]->Effects[i]->ConditionalBeginDestroy();
								BattleAlliesPlayer[Index]->Effects.RemoveAt(i);
								continue;
							}
							BattleAlliesPlayer[Index]->GetMesh()->bPauseAnims = true;
							ContinueTurn = false;
						}
			if (ContinueTurn) {
				//Set animation variables
				UCombatAlliesAnimInstance* CombatAlliesAnimInstance{};
				if (IsValid(BattleAlliesPlayer[Index]))
					if (IsValid(BattleAlliesPlayer[Index]->GetMesh())) {
						if (IsValid(BattleAlliesPlayer[Index]->GetMesh()->GetAnimInstance()))
							CombatAlliesAnimInstance = Cast<UCombatAlliesAnimInstance>(BattleAlliesPlayer[Index]->GetMesh()->GetAnimInstance());
						if (BattleAlliesPlayer[Index]->GetMesh()->bPauseAnims)
							BattleAlliesPlayer[Index]->GetMesh()->bPauseAnims = false;
					}
				if (IsValid(CombatAlliesAnimInstance))
					if (CombatAlliesAnimInstance->GetCombatAlliesIsBlocking())
						CombatAlliesAnimInstance->ToggleCombatAlliesIsBlocking(false);
				for (AEffect* Effect : BattleAlliesPlayer[Index]->Effects) {
					if (Effect->GetEffectType() == EEffectType::TURNSTARTDAMAGE) {
						if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
							if (ATurnStartDamageEffect* TurnStartDamageEffect = Cast<ATurnStartDamageEffect>(Effect); IsValid(TurnStartDamageEffect)) {
								BattleAlliesPlayer[Index]->Execute_GetHit(RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer[Index],
									BattleActions::CalculateAttackValueAfterEffects(Effect->GetEffectStat(), BattleAlliesPlayer[Index]),
									ElementsActions::FindContainedElements(TurnStartDamageEffect->GetSpellElements()), EPhysicalType::NONE, false);
							}
					}
				}
				//Destroy effects, which CurrentDuration is >= Duration.
				for (int i = BattleAlliesPlayer[Index]->Effects.Num() - 1; i >= 0; i--) {
					if (IsValid(BattleAlliesPlayer[Index]->Effects[i])) {
						if (BattleAlliesPlayer[Index]->Effects[i]->GetEffectType() != EEffectType::TURNSKIP) {
							BattleAlliesPlayer[Index]->Effects[i]->CurrentDuration += 1;
							if (BattleAlliesPlayer[Index]->Effects[i]->CurrentDuration >= BattleAlliesPlayer[Index]->Effects[i]->GetDuration() + 1) {
								if (BattleAlliesPlayer[Index]->Effects[i]->GetEffectType() == EEffectType::DIZZINESS) {
									if (IsValid(BattleAlliesPlayer[Index]->DizzyEmitterComponent))
										BattleAlliesPlayer[Index]->DizzyEmitterComponent->DestroyComponent();
								}
								else if (BattleAlliesPlayer[Index]->Effects[i]->GetEffectType() == EEffectType::TURNSTARTDAMAGE) {
									if (IsValid(BattleAlliesPlayer[Index]->FlamesEmitterComponent))
										BattleAlliesPlayer[Index]->FlamesEmitterComponent->DestroyComponent();
								}
								BattleAlliesPlayer[Index]->Effects[i]->ConditionalBeginDestroy();
								BattleAlliesPlayer[Index]->Effects.RemoveAt(i);
							}
						}
					}
				}
			}
		}
	}
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
	if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
		if (UBattleMenu* BattleMenu = Cast<UBattleMenu>(UIManagerWorldSubsystem->BattleMenuWidget); IsValid(BattleMenu)){
			if (IsValid(BattleMenu)) {
				if (!BattleMenu->IsInViewport())
					BattleMenu->AddToViewport();
				BattleMenu->IsChoosingAction = true;
				BattleMenu->IsChoosingSpell = false;
				BattleMenu->IsAttackingWithSpell = false;
				BattleMenu->IsPreparingToAttack = false;
				BattleMenu->IsAttackingWithRange = false;
				BattleMenu->IsPreparingToTalk = false;
				BattleMenu->IsPreparingToViewInfo = false;
				BattleMenu->IsChoosingItem = false;
				BattleMenu->IsAttackingWithItem = false;
				BattleMenu->IsAttackingWithMelee = false;
				BattleMenu->IsChoosingSkill = false;
				BattleMenu->IsChoosingLearnedSpell = false;
				BattleMenu->GetMenuBorder()->SetVisibility(ESlateVisibility::Visible);
				if(IsValid(GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>()))
					GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>()->PickedButton = BattleMenu->GetAttackButton();
				BattleMenu->GetAttackButton()->SetBackgroundColor(FLinearColor(1, 0, 0, 1));
			}
			if (CurrentTurnCombatNPCIndex >= 0) {
				//Destroy effects, which CurrentDuration is >= Duration.
				for (int i = BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects.Num() - 1; i >= 0; i--) {
					if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i])) {
						if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetEffectType() != EEffectType::TURNSKIP) {
							BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration += 1;
							if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->CurrentDuration >= BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetDuration() + 1) {
								if (BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->Effects[i]->GetEffectType() == EEffectType::DIZZINESS) {
									if (ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetController()); IsValid(AIController))
										if (IsValid(AIController->GetBlackboardComponent()))
											AIController->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("CanTargetEveryone", false);
									if (IsValid(BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->DizzyEmitterComponent))
										BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->DizzyEmitterComponent->DestroyComponent();
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
				}
				UIManagerWorldSubsystem->SkillBattleMenuWidget->ResetSkillsScrollBox();
				for (TSubclassOf<ASpell> SpellClass : BattleAlliesPlayer[CurrentTurnCombatNPCIndex]->GetAvailableSpells())
					UIManagerWorldSubsystem->SkillBattleMenuWidget->AddSkillEntryToSkillsScrollBox(Cast<ASpell>(SpellClass->GetDefaultObject()));
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
	//set TargetActor and exit the loop.
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

void ABattleManager::PlayerDeathLogic()
{
	FTimerHandle PlayerDeathLogicTimerHandle{};
	GetWorld()->GetTimerManager().SetTimer(PlayerDeathLogicTimerHandle, this, &ABattleManager::PlayerDeathLogicOnTimer, 1.2f, false);
}

void ABattleManager::PlayerDeathLogicOnTimer()
{
	if (auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem) && IsValid(PlayerCharacter)) {
		UIManagerWorldSubsystem->BattleMenuWidget->RemoveFromParent();
		UIManagerWorldSubsystem->BattleMenuWidget->ConditionalBeginDestroy();
		UIManagerWorldSubsystem->BattleMenuWidget = nullptr;
		UIManagerWorldSubsystem->SpellBattleMenuWidget->ConditionalBeginDestroy();
		UIManagerWorldSubsystem->SpellBattleMenuWidget = nullptr;
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController<APlayerController>(); IsValid(PlayerController))
			if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
				if (IsValid(RedemptionGameModeBase->GetDeathMenuClass()))
					UIManagerWorldSubsystem->DeathMenuWidget = CreateWidget<UDeathMenu>(PlayerController, RedemptionGameModeBase->GetDeathMenuClass());
		if (IsValid(UIManagerWorldSubsystem->DeathMenuWidget))
			UIManagerWorldSubsystem->DeathMenuWidget->AddToViewport();
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
			RedemptionGameModeBase->GetAudioManager()->GetDeathMenuBackgroundMusicAudioComponent()->Play(0.0f);
			RedemptionGameModeBase->GetAudioManager()->GetDeathMenuBackgroundMusicAudioComponent()->SetPaused(false);
			RedemptionGameModeBase->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents
				[RedemptionGameModeBase->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
		}
		for (int8 i = CombatPlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
			if (IsValid(CombatPlayerCharacter->Effects[i])) {
				CombatPlayerCharacter->Effects[i]->ConditionalBeginDestroy();
				CombatPlayerCharacter->Effects.RemoveAt(i);
			}
		}
		if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()); IsValid(PlayerController))
			PlayerController->SetPause(true);
	}
}

void ABattleManager::EndBattle()
{
	//Show results screen
	CurrentTurnCombatNPCIndex = -1;
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
		//Background Music set
		RedemptionGameModeBase->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents
			[RedemptionGameModeBase->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->Play(0.0f);
		RedemptionGameModeBase->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents
			[RedemptionGameModeBase->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(true);
		RedemptionGameModeBase->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->Play(0.0f);
		RedemptionGameModeBase->GetAudioManager()->GetDungeonBattleResultsBackgroundMusicAudioComponent()->SetPaused(false);
		//Widget creation
		if (auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
			UIManagerWorldSubsystem->BattleMenuWidget->RemoveFromParent();
			UIManagerWorldSubsystem->BattleMenuWidget->ConditionalBeginDestroy();
			UIManagerWorldSubsystem->BattleMenuWidget = nullptr;
			if (IsValid(RedemptionGameModeBase->GetBattleResultsScreenClass()))
				UIManagerWorldSubsystem->BattleResultsScreenWidget = CreateWidget<UBattleResultsScreen>(Cast<APlayerController>(PlayerCharacter->GetController()),
					RedemptionGameModeBase->GetBattleResultsScreenClass());
			if (IsValid(UIManagerWorldSubsystem->BattleResultsScreenWidget))
				UIManagerWorldSubsystem->BattleResultsScreenWidget->AddToViewport();
		}
	}
	CanTurnBehindPlayerCameraToTarget = false;
	CanTurnBehindPlayerCameraToSpellObject = false;
	CanTurnBehindPlayerCameraToStartPosition = true;
	if (IsValid(SelectedCombatNPC))
		SelectedCombatNPC->GetFloatingHealthBarWidget()->GetHealthBar()->SetVisibility(ESlateVisibility::Hidden);
	for (int16 i = CombatPlayerCharacter->Effects.Num() - 1; i >= 0; i--) {
		if (IsValid(CombatPlayerCharacter->Effects[i])) {
			CombatPlayerCharacter->Effects[i]->ConditionalBeginDestroy();
			CombatPlayerCharacter->Effects.RemoveAt(i);
		}
	}
	for (ACombatNPC* CombatNPC : PlayerCharacter->GetAllies())
		if (IsValid(CombatNPC->GetSmartObject()))
			CombatNPC->GetSmartObject()->ConditionalBeginDestroy();
}

void ABattleManager::SetCanTurnBehindPlayerCameraToTarget(const bool Value)
{
	CanTurnBehindPlayerCameraToTarget = Value;
}

void ABattleManager::SetCanTurnBehindPlayerCameraToStartPosition(const bool Value)
{
	CanTurnBehindPlayerCameraToStartPosition = Value;
}

void ABattleManager::SetCanTurnBehindPlayerCameraToSpellObject(const bool Value)
{
	CanTurnBehindPlayerCameraToSpellObject = Value;
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

