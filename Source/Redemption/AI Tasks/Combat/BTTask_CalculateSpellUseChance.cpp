// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CalculateSpellUseChance.h"
#include "..\Characters\Combat\CombatEnemyNPC.h"
#include "..\Characters\AI Controllers\Combat\CombatNPCAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

UBTTask_CalculateSpellUseChance::UBTTask_CalculateSpellUseChance(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Calculate a spell use chance";
}

EBTNodeResult::Type UBTTask_CalculateSpellUseChance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(OwnerComp.GetAIOwner()->GetCharacter());
	if (!IsValid(CombatEnemyNPC))
		return EBTNodeResult::Failed;

	int8 UseChance = 60;

	if (!IsValid(CombatEnemyNPC->SpellToUse)) {
		UseChance = -1;
	}
	else {
		if (CombatEnemyNPC->CurrentMana < CombatEnemyNPC->SpellToUse->GetCost()) {
			UseChance = -1;
		}
	}

	uint8 RandomChance = FMath::RandRange(0, 100);
	if (RandomChance <= UseChance) {
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>("CombatAction", uint8(ECombatAction::USESPELL));
		if(CombatEnemyNPC->SpellToUse->GetTypeOfSpell() == ESpellType::ASSAULT || CombatEnemyNPC->SpellToUse->GetTypeOfSpell() == ESpellType::DEBUFF)
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("UseSpellWithSpellObject", true);
		else
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("UseSpellWithSpellObject", false);
	}
	else {
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>("CombatAction", uint8(ECombatAction::MELEEATTACK));
		CombatEnemyNPC->SpellToUse = nullptr;
	}
	return EBTNodeResult::Succeeded;
}

