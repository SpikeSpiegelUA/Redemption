// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_NoSpellObjectSpellUse.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "Redemption/Miscellaneous/SkillsSpellsAndEffectsActions.h"
#include "Redemption/Miscellaneous/ElementsActions.h"
#include "Kismet/GameplayStatics.h"
#include "..\Dynamics\Gameplay\Skills and Effects\PresetBuffSpell.h"
#include "..\UI\HUD\FloatingManaBarWidget.h"

void UAnimNotify_NoSpellObjectSpellUse::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ACombatNPC* Owner = Cast<ACombatNPC>(MeshComp->GetOwner());
	APlayerCharacter* PlayerCharacter{};
	if(IsValid(MeshComp->GetWorld()) && IsValid(MeshComp->GetWorld()->GetFirstPlayerController()))
		PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (!IsValid(Owner) || !IsValid(PlayerCharacter))
		return;

	//Get Target actors, depending on a caster battle side and a spell's range, and run the logic on each target.
	TArray<ACombatNPC*> TargetsArray{};
	if (IsValid(Cast<ACombatAllies>(Owner)))
		TargetsArray = SkillsSpellsAndEffectsActions::GetTargetsForAllies(PlayerCharacter->GetBattleManager(), Owner->SpellToUse->GetSpellRange(), Owner->SpellToUse->GetTypeOfSpell());
	else if (IsValid(Cast<ACombatEnemyNPC>(Owner)))
		TargetsArray = SkillsSpellsAndEffectsActions::GetTargetsForEnemies(PlayerCharacter->GetBattleManager(), Owner->Target, Owner->SpellToUse->GetSpellRange(), Owner->SpellToUse->GetTypeOfSpell());

	if (ARestorationSpell* SpellToUse = Cast<ARestorationSpell>(Owner->SpellToUse); IsValid(SpellToUse)) {
		bool SpellHasBeenUsed = false;
		for (ACombatNPC* UseTarget : TargetsArray) {
			if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::HEALTH && UseTarget->CurrentHP < UseTarget->MaxHP && UseTarget->CurrentHP > 0) {
				int16 AmountToHeal = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(UseTarget->MaxHP * SpellToUse->GetRestorationValuePercent() / 100,
					UseTarget->Effects, UseTarget->GetElementalResistances(), ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()));
				UseTarget->CurrentHP += AmountToHeal;
				ACombatFloatingInformationActor* CombatFloatingInformationActor = MeshComp->GetWorld()->
					SpawnActor<ACombatFloatingInformationActor>(PlayerCharacter->GetBattleManager()->GetCombatFloatingInformationActorClass(),
						UseTarget->GetActorLocation(), UseTarget->GetActorRotation());
				FString TextForCombatFloatingInformationActor = FString();
				TextForCombatFloatingInformationActor.Append("+");
				TextForCombatFloatingInformationActor.AppendInt(AmountToHeal);
				CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
				UseTarget->GetFloatingHealthBarWidget()->HP = UseTarget->CurrentHP;
				SpellHasBeenUsed = true;
				if (UseTarget->CurrentHP > UseTarget->MaxHP)
					UseTarget->CurrentHP = UseTarget->MaxHP;
			}
			else if (SpellToUse->GetTypeOfRestoration() == ESpellRestorationType::MANA && UseTarget->CurrentMana < UseTarget->MaxMana && UseTarget->CurrentHP > 0) {
				int16 AmountToRestore = SkillsSpellsAndEffectsActions::GetRestorationValueAfterResistances(UseTarget->MaxMana * SpellToUse->GetRestorationValuePercent() / 100,
					UseTarget->Effects, UseTarget->GetElementalResistances(), ElementsActions::FindContainedElements(SpellToUse->GetSpellElements()));
				UseTarget->CurrentMana += AmountToRestore;
				ACombatFloatingInformationActor* CombatFloatingInformationActor = MeshComp->GetWorld()->
					SpawnActor<ACombatFloatingInformationActor>(PlayerCharacter->GetBattleManager()->GetCombatFloatingInformationActorClass(), UseTarget->GetActorLocation(), UseTarget->GetActorRotation());
				FString TextForCombatFloatingInformationActor = FString();
				TextForCombatFloatingInformationActor.Append("+");
				TextForCombatFloatingInformationActor.AppendInt(AmountToRestore);
				CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
				if (ACombatAllies* AllyTarget = Cast<ACombatAllies>(UseTarget); IsValid(AllyTarget))
					AllyTarget->GetFloatingManaBarWidget()->Mana = UseTarget->CurrentMana;
				SpellHasBeenUsed = true;
				if (UseTarget->CurrentMana > UseTarget->MaxMana)
					UseTarget->CurrentMana = UseTarget->MaxMana;
			}
		}
		UGameplayStatics::PlaySound2D(MeshComp->GetWorld(), PlayerCharacter->GetAudioManager()->GetUseHealOrBuffSoundCue());
		if (SpellToUse->GetSpellCostType() == ESpellCostType::MANA) {
			Owner->CurrentMana -= SpellToUse->GetCost();
			if (Owner->CurrentMana < 0)
				Owner->CurrentMana = 0;
		}
		else {
			Owner->CurrentHP -= SpellToUse->GetCost();
			if (Owner->CurrentHP < 0)
				Owner->CurrentHP = 0;
		}
	}
	else if (APresetBuffSpell* PresetBuffSpellToUse = Cast<APresetBuffSpell>(Owner->SpellToUse); IsValid(PresetBuffSpellToUse)) {
		for (ACombatNPC* TargetActor : TargetsArray) {
			if (TargetActor->CurrentHP > 0) {
				TArray<AEffect*> CreatedEffectsFromClasses{};
				for (TSubclassOf<AEffect> EffectClass : PresetBuffSpellToUse->GetEffectsClasses()) {
					AEffect* NewEffect = NewObject<AEffect>(this, EffectClass);
					CreatedEffectsFromClasses.Add(NewEffect);
				}
				TargetActor->Execute_GetHitWithBuffOrDebuff(TargetActor, CreatedEffectsFromClasses, ElementsActions::FindContainedElements(PresetBuffSpellToUse->GetSpellElements()), PresetBuffSpellToUse->GetTypeOfSpell());
			}
		}
		UGameplayStatics::PlaySound2D(MeshComp->GetWorld(), PlayerCharacter->GetAudioManager()->GetUseHealOrBuffSoundCue());
		if (PresetBuffSpellToUse->GetSpellCostType() == ESpellCostType::MANA) {
			Owner->CurrentMana -= PresetBuffSpellToUse->GetCost();
			if (Owner->CurrentMana < 0)
				Owner->CurrentMana = 0;
		}
		else {
			Owner->CurrentHP -= PresetBuffSpellToUse->GetCost();
			if (Owner->CurrentHP < 0)
				Owner->CurrentHP = 0;
		}
	}
}
