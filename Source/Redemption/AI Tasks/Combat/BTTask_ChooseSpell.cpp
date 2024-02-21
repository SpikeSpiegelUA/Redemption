// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChooseSpell.h"
#include "..\Characters\Combat\CombatEnemyNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"

UBTTask_ChooseSpell::UBTTask_ChooseSpell(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Choose a spell";
}

EBTNodeResult::Type UBTTask_ChooseSpell::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	ACombatNPC* CombatNPC = Cast<ACombatNPC>(OwnerComp.GetAIOwner()->GetCharacter());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(CombatNPC) || !IsValid(MyBlackboard) || !IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	//Get actors

	//Each spell type has its indexes. Add index to this array, if a spell of a particular type is available. Then choose random index, means choose random spell type.
	TArray<ESpellType> AvailableSpellsIndex{};
	bool RestorationAdded = false;
	//Need this to control whether to use several targets restoration spell.
	uint8 NumberOfDamagedEnemies = 0;
	for (TSubclassOf<ASpell> SpellClass : CombatNPC->GetAvailableSpells()) {
		ASpell* SpellDefaultObject = SpellClass->GetDefaultObject<ASpell>();
		//Get targets for CurrentHP check.
		TArray<ACombatNPC*> TargetsArray{};
		if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
			if (IsValid(Cast<ACombatEnemyNPC>(CombatNPC))) {
				if (SpellDefaultObject->GetTypeOfSpell() == ESpellType::RESTORATION || SpellDefaultObject->GetTypeOfSpell() == ESpellType::BUFF)
					TargetsArray = RedemptionGameModeBase->GetBattleManager()->BattleEnemies;
				else if (SpellDefaultObject->GetTypeOfSpell() == ESpellType::ASSAULT || SpellDefaultObject->GetTypeOfSpell() == ESpellType::DEBUFF)
					TargetsArray = RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer;
			}
			else if (IsValid(Cast<ACombatAllies>(CombatNPC))) {
				if (SpellDefaultObject->GetTypeOfSpell() == ESpellType::RESTORATION || SpellDefaultObject->GetTypeOfSpell() == ESpellType::BUFF)
					TargetsArray = RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer;
				else if (SpellDefaultObject->GetTypeOfSpell() == ESpellType::ASSAULT || SpellDefaultObject->GetTypeOfSpell() == ESpellType::DEBUFF)
					TargetsArray = RedemptionGameModeBase->GetBattleManager()->BattleEnemies;
			}
		}
		switch (SpellDefaultObject->GetTypeOfSpell()) {
			case ESpellType::RESTORATION:
				/*for (ACombatNPC* Target : TargetsArray) {
					if (Target->CurrentHP / Target->MaxHP * 100.f <= 50.f && Target->CurrentHP > 0) {
						if (!RestorationAdded) {
							AvailableSpellsIndex.Add(ESpellType::RESTORATION);
							AvailableSpellsIndex.Add(ESpellType::RESTORATION);
							RestorationAdded = true;
						}
						NumberOfDamagedEnemies++;
					}
				}*/
				AvailableSpellsIndex.AddUnique(ESpellType::RESTORATION);
				break;
			case ESpellType::ASSAULT:
				AvailableSpellsIndex.AddUnique(ESpellType::ASSAULT);
				break;
			case ESpellType::BUFF:
				AvailableSpellsIndex.AddUnique(ESpellType::BUFF);
				break;
			case ESpellType::DEBUFF:
				AvailableSpellsIndex.AddUnique(ESpellType::DEBUFF);
				break;
		}
	}

	ESpellType SpellTypeToUse{};

	if (AvailableSpellsIndex.Num() > 0) 
		SpellTypeToUse = AvailableSpellsIndex[FMath::RandRange(0, AvailableSpellsIndex.Num() - 1)];
	else {
		CombatNPC->SpellToUse = nullptr;
		return EBTNodeResult::Succeeded;
	}
	TArray<ASpell*> SingleRangeSpellsToUse{};
	TArray<ASpell*> NeighborsOrEveryoneRangeSpellsToUse{};

	//Fill declared above array depending on a spell's range.
	for (TSubclassOf<ASpell> SpellClass : CombatNPC->GetAvailableSpells()) {
		ASpell* SpellDefaultObject = SpellClass->GetDefaultObject<ASpell>();
		if (SpellDefaultObject->GetTypeOfSpell() == SpellTypeToUse) {
			if (SpellDefaultObject->GetSpellRange() == ESpellRange::SINGLE)
				SingleRangeSpellsToUse.Add(SpellDefaultObject);
			else if (SpellDefaultObject->GetSpellRange() == ESpellRange::NEIGHBORS || SpellDefaultObject->GetSpellRange() == ESpellRange::EVERYONE)
				NeighborsOrEveryoneRangeSpellsToUse.Add(SpellDefaultObject);
		}
	}
	//For example, we use multiple range restoration spells, only if there are more than 1 wounded targets. That is a special case. 
	// If false, then choose random spell from all spells, if true, then end the logic.
	//Get targets for a number of targets check.
	TArray<ACombatNPC*> TargetsArray{};
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
		if (IsValid(Cast<ACombatEnemyNPC>(CombatNPC))) {
			if (SpellTypeToUse == ESpellType::RESTORATION || SpellTypeToUse == ESpellType::BUFF)
				TargetsArray = RedemptionGameModeBase->GetBattleManager()->BattleEnemies;
			else if (SpellTypeToUse == ESpellType::ASSAULT || SpellTypeToUse == ESpellType::DEBUFF)
				TargetsArray = RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer;
		}
		else if (IsValid(Cast<ACombatAllies>(CombatNPC))) {
			if (SpellTypeToUse == ESpellType::RESTORATION || SpellTypeToUse == ESpellType::BUFF)
				TargetsArray = RedemptionGameModeBase->GetBattleManager()->BattleAlliesPlayer;
			else if (SpellTypeToUse == ESpellType::ASSAULT || SpellTypeToUse == ESpellType::DEBUFF)
				TargetsArray = RedemptionGameModeBase->GetBattleManager()->BattleEnemies;
		}
	}
	bool SpecialCaseSpellSelected = false;
	if (NeighborsOrEveryoneRangeSpellsToUse.Num() > 0) {
		if (SpellTypeToUse == ESpellType::RESTORATION && NumberOfDamagedEnemies >= 2) {
			CombatNPC->SpellToUse = NeighborsOrEveryoneRangeSpellsToUse[FMath::RandRange(0, NeighborsOrEveryoneRangeSpellsToUse.Num() - 1)];
			SpecialCaseSpellSelected = true;
		}
		else if (SpellTypeToUse == ESpellType::ASSAULT) {
			//if()
		}
	}
	if (!SpecialCaseSpellSelected) {
		TArray<ASpell*> AllRangesSpells{};
		for (ASpell* SingleRangeSpell : SingleRangeSpellsToUse)
			AllRangesSpells.Add(SingleRangeSpell);
		for (ASpell* MultipleRangeSpell : NeighborsOrEveryoneRangeSpellsToUse)
			AllRangesSpells.Add(MultipleRangeSpell);
		if (AllRangesSpells.Num() > 0)
			CombatNPC->SpellToUse = AllRangesSpells[FMath::RandRange(0, AllRangesSpells.Num() - 1)];
		else
			CombatNPC->SpellToUse = nullptr;
	}
	return EBTNodeResult::Succeeded;
}

