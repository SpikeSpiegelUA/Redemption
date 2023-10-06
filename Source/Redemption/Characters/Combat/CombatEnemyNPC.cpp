// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatEnemyNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\Combat\CombatEnemyNPCAnimInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ElementsActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Combat\CombatFloatingInformationActor.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Redemption/Characters/AI Controllers/Combat/CombatEnemyNPCAIController.h>

// Sets default values
ACombatEnemyNPC::ACombatEnemyNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACombatEnemyNPC::BeginPlay()
{
	Super::BeginPlay();
}
// Called every frame
void ACombatEnemyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatEnemyNPC::GetHitWithBuffOrDebuff_Implementation(const TArray<class AEffect*>& HitEffects)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager)) {
			if (IsValid(PlayerCharacter->GetSpellBattleMenuWidget()->GetCreatedSpell())) {
				ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(),
					GetActorLocation(), GetActorRotation());
				FString TextForCombatFloatingInformationActor = FString();
				uint8 EvasionRandomNumber = FMath::RandRange(0, 100);
				int ChanceOfEvasion = SkillsSpellsAndEffectsActions::GetBuffOrDebuffEvasionChanceAfterResistances(SkillsSpellsAndEffectsActions::GetValueAfterEffects(EvasionChance + Agility * 2,
					Effects, EEffectArea::EVASION), Effects, Resistances, PlayerCharacter->GetSpellBattleMenuWidget()->GetCreatedSpell()->GetElementsAndTheirPercentagesStructs());
				if (EvasionRandomNumber <= ChanceOfEvasion) {
					TextForCombatFloatingInformationActor.Append("Miss!");
					CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
				}
				else {
					TextForCombatFloatingInformationActor.Append("Hit!");
					CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
					for (AEffect* Effect : HitEffects)
						Effects.Add(Effect);
				}
			}
		}
}

void ACombatEnemyNPC::GetHit_Implementation(int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements, bool ForcedMiss)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager)) {
			ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), GetActorLocation(), GetActorRotation());
			FString TextForCombatFloatingInformationActor = FString();
			uint8 EvasionRandomNumber = FMath::RandRange(0, 100);
			int ChanceOfEvasion = SkillsSpellsAndEffectsActions::GetValueAfterEffects(EvasionChance + Agility * 2, Effects, EEffectArea::EVASION);
			if (EvasionRandomNumber <= ChanceOfEvasion || ForcedMiss) {
				TextForCombatFloatingInformationActor.Append("Miss!");
				CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
			}
			else {
				int ValueOfArmor = SkillsSpellsAndEffectsActions::GetValueAfterEffects(ArmorValue, Effects, EEffectArea::ARMOR);
				int ValueOfAttackWithResistances = SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(ValueOfAttack, Effects, Resistances, ContainedElements);
				if (CurrentHP - (ValueOfAttackWithResistances - ValueOfArmor / 10) < 0)
					CurrentHP = 0;
				else
					CurrentHP -= (ValueOfAttackWithResistances - ValueOfArmor / 10);
				if (IsValid(FloatingHealthBarWidget))
					FloatingHealthBarWidget->HP = CurrentHP;
				TextForCombatFloatingInformationActor.AppendInt(ValueOfAttackWithResistances - ValueOfArmor / 10);
				CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
			}
			UCombatEnemyNPCAnimInstance* AnimInstance = Cast<UCombatEnemyNPCAnimInstance>(GetMesh()->GetAnimInstance());
			if (IsValid(AnimInstance)) {
				if (CurrentHP <= 0)
					AnimInstance->ToggleCombatCharacterIsDead(true);
				else
					AnimInstance->ToggleCombatCharacterGotHit(true);
			}
		}
}

void ACombatEnemyNPC::StartADialogue_Implementation()
{
	ACombatEnemyNPCAIController* AIController = Cast<ACombatEnemyNPCAIController>(GetController());
	if (IsValid(AIController))
		AIController->StartADialogue();
}

int ACombatEnemyNPC::GetGoldReward() const
{
	return GoldReward;
}

UBehaviorTree* ACombatEnemyNPC::GetDialogueTree()
{
	return DialogueTree;
}
