// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetTarget.h"
#include "AIController.h"
#include "..\Characters\Combat\CombatNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "Redemption/Miscellaneous/SkillsSpellsAndEffectsActions.h"
#include "Redemption/Miscellaneous/ElementsActions.h"
#include "Kismet/GameplayStatics.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"

UBTTask_GetTarget::UBTTask_GetTarget(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Target";
}

EBTNodeResult::Type UBTTask_GetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (!IsValid(MyController))
		return EBTNodeResult::Failed;

	ACombatNPC* Chr = Cast<ACombatNPC>(MyController->GetPawn());
	if (!IsValid(Chr))
		return EBTNodeResult::Failed;

	ASmartObject* SmartObject = Cast<ASmartObject>(Chr->GetSmartObject());
	if (!IsValid(SmartObject))
		return EBTNodeResult::Failed;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if(!IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;
	
	ABattleManager* BattleManager{};
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		BattleManager = RedemptionGameModeBase->GetBattleManager();
	else
		return EBTNodeResult::Failed;

	TArray<ACombatNPC*> ActorsToTarget{};
	if (OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Bool>("CanTargetEveryone")) {
		for (ACombatNPC* Actor : BattleManager->BattleAlliesPlayer)
			if(Actor != MyController->GetPawn() && Actor->CurrentHP > 0)
				ActorsToTarget.Add(Actor);
		for (ACombatNPC* Actor : BattleManager->BattleEnemies)
			if (Actor != MyController->GetPawn() && Actor->CurrentHP > 0)
				ActorsToTarget.Add(Actor);
	}
	else {
		if (IsValid(Chr->SpellToUse)) {
			if (Chr->SpellToUse->GetTypeOfSpell() == ESpellType::ASSAULT || Chr->SpellToUse->GetTypeOfSpell() == ESpellType::DEBUFF) {
				if (IsValid(Cast<ACombatAllies>(Chr)))
					for (ACombatNPC* Actor : BattleManager->BattleEnemies)
						if (Actor->CurrentHP > 0)
							ActorsToTarget.Add(Actor);
				if (IsValid(Cast<ACombatEnemyNPC>(Chr)))
					for (ACombatNPC* Actor : BattleManager->BattleAlliesPlayer)
						if (Actor->CurrentHP > 0)
							ActorsToTarget.Add(Actor);
			}
			else if (Chr->SpellToUse->GetTypeOfSpell() == ESpellType::RESTORATION || Chr->SpellToUse->GetTypeOfSpell() == ESpellType::BUFF) {
				if (IsValid(Cast<ACombatAllies>(Chr)))
					for (ACombatNPC* Actor : BattleManager->BattleAlliesPlayer)
						if (Actor->CurrentHP > 0)
							ActorsToTarget.Add(Actor);
				if (IsValid(Cast<ACombatEnemyNPC>(Chr)))
					for (ACombatNPC* Actor : BattleManager->BattleEnemies)
						if (Actor->CurrentHP > 0)
							ActorsToTarget.Add(Actor);
			}
		}
		else {
			if (IsValid(Cast<ACombatAllies>(Chr)))
				for (ACombatNPC* Actor : BattleManager->BattleEnemies)
					if (Actor->CurrentHP > 0)
						ActorsToTarget.Add(Actor);
			if (IsValid(Cast<ACombatEnemyNPC>(Chr)))
				for (ACombatNPC* Actor : BattleManager->BattleAlliesPlayer)
					if (Actor->CurrentHP > 0)
						ActorsToTarget.Add(Actor);
		}
	}

	//IMPORTANT!!! If you have selected a target in one of the branches, then set this to false. 
	//Otherwise your pointer to the selected target will be overrided by pointer to a randomly selected target.
	bool SelectRandomTarget = true;
	if (IsValid(Chr->SpellToUse)) {
		//Here it gets a bit complex. The Owner needs to know, which target to choose for restoration spell. If there are 3 or 4 targets, then we need
		//to calculate, where a restoration spell will be the most effective.
		if (Chr->SpellToUse->GetSpellRange() == ESpellRange::NEIGHBORS) {
			if (Chr->SpellToUse->GetTypeOfSpell() == ESpellType::RESTORATION) {
				if (ActorsToTarget.Num() == 1) {
					Chr->Target = ActorsToTarget[0];
					OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", ActorsToTarget[0]);
					SelectRandomTarget = false;
				}
				else if (ActorsToTarget.Num() == 2) {
					if (ActorsToTarget[0]->CurrentHP / ActorsToTarget[0]->MaxHP * 100.f < ActorsToTarget[1]->CurrentHP / ActorsToTarget[1]->MaxHP * 100.f) {
						Chr->Target = ActorsToTarget[0];
						OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", ActorsToTarget[0]);
						SelectRandomTarget = false;
					}
					else {
						Chr->Target = ActorsToTarget[1];
						OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", ActorsToTarget[1]);
						SelectRandomTarget = false;
					}
				}
				else if (ActorsToTarget.Num() == 3 || ActorsToTarget.Num() == 4) {
					//Okay, here goes the priority algorithm.
					if (ARestorationSpell* RestorationSpell = Cast<ARestorationSpell>(Chr->SpellToUse); IsValid(RestorationSpell)) {
						//uint8* TargetPriorities = new uint8[ActorsToTarget.Num()];
						auto TargetPriorities = std::make_unique<uint8[]>(ActorsToTarget.Num());
						for (uint8 Index = 0; Index < ActorsToTarget.Num(); Index++)
							TargetPriorities[Index] = 0;
						//Find a target with the lowest CurrentHP percent.
						uint8 LowestHPPercentIndex{};
						float MinHP = 100.f;
						for(uint8 Index = 0; Index < ActorsToTarget.Num(); Index++)
							if (ActorsToTarget[Index]->CurrentHP / ActorsToTarget[Index]->MaxHP * 100.f < MinHP) {
								MinHP = ActorsToTarget[Index]->CurrentHP / ActorsToTarget[Index]->MaxHP * 100.f;
								LowestHPPercentIndex = Index;
							}
						TargetPriorities[LowestHPPercentIndex] += 1;
						//Find a target, that will use most of spell's healing points;
						//We need to know about dead allies/enemies also, so we choose either BattleEnemies or BattleAlliesPlayer.
						TArray<ACombatNPC*> TargetsWithDeadAlso{};
						if (IsValid(Cast<ACombatAllies>(Chr)))
							TargetsWithDeadAlso = BattleManager->BattleAlliesPlayer;
						if (IsValid(Cast<ACombatEnemyNPC>(Chr)))
							TargetsWithDeadAlso = BattleManager->BattleEnemies;
						//Now we need to find, which target will restore the largest amount of HP. Also account for >MaxHP.
						float HighestRestorationAmount = 0.f;
						uint8 HighestRestorationAmountTargetIndex{};
						for (uint8 Index = 0; Index < TargetsWithDeadAlso.Num(); Index++) {
							if (TargetsWithDeadAlso[Index]->CurrentHP > 0) {
								float CurrentRestorationAmount = 0.f;
								if (Index == 0) {
									if (TargetsWithDeadAlso[Index]->CurrentHP > 0) {
										int16 IndexZeroAmountToHeal = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(TargetsWithDeadAlso[Index]->MaxHP * RestorationSpell->GetRestorationValuePercent() / 100,
											TargetsWithDeadAlso[Index]->Effects, TargetsWithDeadAlso[Index]->GetElementalResistances(), ElementsActions::FindContainedElements(RestorationSpell->GetSpellElements()));
										if (TargetsWithDeadAlso[Index]->CurrentHP + IndexZeroAmountToHeal > TargetsWithDeadAlso[Index]->MaxHP)
											CurrentRestorationAmount += TargetsWithDeadAlso[Index]->MaxHP - TargetsWithDeadAlso[Index]->CurrentHP;
										else
											CurrentRestorationAmount += IndexZeroAmountToHeal;
									}
									if (TargetsWithDeadAlso.Num() > 1) {
										if (TargetsWithDeadAlso[Index + 1]->CurrentHP > 0) {
											int16 IndexPlusOneAmountToHeal = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(TargetsWithDeadAlso[Index + 1]->MaxHP * RestorationSpell->GetRestorationValuePercent() / 100,
												TargetsWithDeadAlso[Index + 1]->Effects, TargetsWithDeadAlso[Index + 1]->GetElementalResistances(), ElementsActions::FindContainedElements(RestorationSpell->GetSpellElements()));
											if (TargetsWithDeadAlso[Index + 1]->CurrentHP + IndexPlusOneAmountToHeal > TargetsWithDeadAlso[Index + 1]->MaxHP)
												CurrentRestorationAmount += TargetsWithDeadAlso[Index + 1]->MaxHP - TargetsWithDeadAlso[Index + 1]->CurrentHP;
											else
												CurrentRestorationAmount += IndexPlusOneAmountToHeal;
										}
									}
								}
								else if (Index == TargetsWithDeadAlso.Num() - 1) {
									if (TargetsWithDeadAlso[Index]->CurrentHP > 0) {
										int16 IndexAmountToHeal = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(TargetsWithDeadAlso[Index]->MaxHP * RestorationSpell->GetRestorationValuePercent() / 100,
											TargetsWithDeadAlso[Index]->Effects, TargetsWithDeadAlso[Index]->GetElementalResistances(), ElementsActions::FindContainedElements(RestorationSpell->GetSpellElements()));
										if (TargetsWithDeadAlso[Index]->CurrentHP + IndexAmountToHeal > TargetsWithDeadAlso[Index]->MaxHP)
											CurrentRestorationAmount += TargetsWithDeadAlso[Index]->MaxHP - TargetsWithDeadAlso[Index]->CurrentHP;
										else
											CurrentRestorationAmount += IndexAmountToHeal;
									}
									if (TargetsWithDeadAlso[Index - 1]->CurrentHP > 0) {
										int16 IndexMinusOneAmountToHeal = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(TargetsWithDeadAlso[Index - 1]->MaxHP * RestorationSpell->GetRestorationValuePercent() / 100,
											TargetsWithDeadAlso[Index - 1]->Effects, TargetsWithDeadAlso[Index - 1]->GetElementalResistances(), ElementsActions::FindContainedElements(RestorationSpell->GetSpellElements()));
										if (TargetsWithDeadAlso[Index - 1]->CurrentHP + IndexMinusOneAmountToHeal > TargetsWithDeadAlso[Index - 1]->MaxHP)
											CurrentRestorationAmount += TargetsWithDeadAlso[Index - 1]->MaxHP - TargetsWithDeadAlso[Index - 1]->CurrentHP;
										else
											CurrentRestorationAmount += IndexMinusOneAmountToHeal;
									}
								}
								else {
									if (TargetsWithDeadAlso[Index]->CurrentHP > 0) {
										int16 IndexAmountToHeal = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(TargetsWithDeadAlso[Index]->MaxHP * RestorationSpell->GetRestorationValuePercent() / 100,
											TargetsWithDeadAlso[Index]->Effects, TargetsWithDeadAlso[Index]->GetElementalResistances(), ElementsActions::FindContainedElements(RestorationSpell->GetSpellElements()));
										if (TargetsWithDeadAlso[Index]->CurrentHP + IndexAmountToHeal > TargetsWithDeadAlso[Index]->MaxHP)
											CurrentRestorationAmount += TargetsWithDeadAlso[Index]->MaxHP - TargetsWithDeadAlso[Index]->CurrentHP;
										else
											CurrentRestorationAmount += IndexAmountToHeal;
									}
									if (TargetsWithDeadAlso[Index - 1]->CurrentHP > 0) {
										int16 IndexMinusOneAmountToHeal = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(TargetsWithDeadAlso[Index - 1]->MaxHP * RestorationSpell->GetRestorationValuePercent() / 100,
											TargetsWithDeadAlso[Index - 1]->Effects, TargetsWithDeadAlso[Index - 1]->GetElementalResistances(), ElementsActions::FindContainedElements(RestorationSpell->GetSpellElements()));
										if (TargetsWithDeadAlso[Index - 1]->CurrentHP + IndexMinusOneAmountToHeal > TargetsWithDeadAlso[Index - 1]->MaxHP)
											CurrentRestorationAmount += TargetsWithDeadAlso[Index - 1]->MaxHP - TargetsWithDeadAlso[Index - 1]->CurrentHP;
										else
											CurrentRestorationAmount += IndexMinusOneAmountToHeal;
									}
									if (TargetsWithDeadAlso[Index + 1]->CurrentHP > 0) {
										int16 IndexPlusOneAmountToHeal = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(TargetsWithDeadAlso[Index + 1]->MaxHP * RestorationSpell->GetRestorationValuePercent() / 100,
											TargetsWithDeadAlso[Index + 1]->Effects, TargetsWithDeadAlso[Index + 1]->GetElementalResistances(), ElementsActions::FindContainedElements(RestorationSpell->GetSpellElements()));
										if (TargetsWithDeadAlso[Index + 1]->CurrentHP + IndexPlusOneAmountToHeal > TargetsWithDeadAlso[Index + 1]->MaxHP)
											CurrentRestorationAmount += TargetsWithDeadAlso[Index + 1]->MaxHP - TargetsWithDeadAlso[Index + 1]->CurrentHP;
										else
											CurrentRestorationAmount += IndexPlusOneAmountToHeal;
									}
								}
								if (CurrentRestorationAmount > HighestRestorationAmount) {
									HighestRestorationAmount = CurrentRestorationAmount;
									HighestRestorationAmountTargetIndex = Index;
								}
							}
						}
						TargetPriorities[HighestRestorationAmountTargetIndex] += 1;
						uint8 HighestPriority = 0;
						uint8 HighestPriorityIndex = 0;
						for (uint8 IndexPriority = 0; IndexPriority < ActorsToTarget.Num(); IndexPriority++) {
							if (TargetPriorities[IndexPriority] > HighestPriority) {
								HighestPriority = TargetPriorities[IndexPriority];
								HighestPriorityIndex = IndexPriority;
							}
						}
						Chr->Target = ActorsToTarget[HighestPriorityIndex];
						OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", ActorsToTarget[HighestPriorityIndex]);
						SelectRandomTarget = false;
					}
				}
			}
		}
		else if (Chr->SpellToUse->GetSpellRange() == ESpellRange::EVERYONE) {
			Chr->Target = ActorsToTarget[0];
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", ActorsToTarget[0]);
			SelectRandomTarget = false;
		}
		else if(Chr->SpellToUse->GetSpellRange() == ESpellRange::SINGLE) {
			if (Chr->SpellToUse->GetTypeOfSpell() == ESpellType::RESTORATION) {
				for (ACombatNPC* CombatNPC : ActorsToTarget)
					if (CombatNPC->CurrentHP / CombatNPC->MaxHP * 100.f <= 50.f) {
						Chr->Target = CombatNPC;
						OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", CombatNPC);
						SelectRandomTarget = false;
						break;
					}
			}
		}
	}
	if (SelectRandomTarget) {
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
				Chr->Target = ActorsToTarget[i];
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>("TargetActor", ActorsToTarget[i]);
				break;
			}
		}
	}

	return EBTNodeResult::Succeeded;
}
