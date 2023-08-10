// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatEnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\CombatNPCAnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ElementsActions.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Combat\CombatFloatingInformationActor.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ACombatEnemyNPC::ACombatEnemyNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Create widget for EnemyHealthBar
	EnemyHealthBarComponentWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	EnemyHealthBarComponentWidget->SetWidgetSpace(EWidgetSpace::Screen);
	EnemyHealthBarComponentWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACombatEnemyNPC::BeginPlay()
{
	Super::BeginPlay();
	//Set up properties for EnemyHealthBarWidget
	EnemyHealthBarComponentWidget->SetWidgetClass(EnemyHealthBarClass);
	EnemyHealthBarWidget = Cast<UEnemyHealthBarWidget>(EnemyHealthBarComponentWidget->GetWidget());
	if (IsValid(EnemyHealthBarWidget)) {
		EnemyHealthBarWidget->HP = HP;
		EnemyHealthBarWidget->MaxHP = HP;
	}
}
// Called every frame
void ACombatEnemyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatEnemyNPC::GetHitWithBuffOrDebuff_Implementation(const TArray<class AEffect*>& HitEffects)
{
	for (AEffect* Effect : HitEffects) 
		Effects.Add(Effect);
}

void ACombatEnemyNPC::GetHit_Implementation(int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager)) {
			ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), GetActorLocation(), GetActorRotation());
			FString TextForCombatFloatingInformationActor = FString();
			uint8 EvasionRandomNumber = FMath::RandRange(0, 100);
			int ChanceOfEvasion = SkillsSpellsAndEffectsActions::GetValueAfterEffects(EvasionChance + Agility * 2, Effects, EEffectArea::EVASION);
			if (EvasionRandomNumber <= ChanceOfEvasion) {
				TextForCombatFloatingInformationActor.Append("Miss!");
				CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
			}
			else {
				int ValueOfArmor = SkillsSpellsAndEffectsActions::GetValueAfterEffects(ArmorValue, Effects, EEffectArea::ARMOR);
				int ValueOfAttackWithResistances = SkillsSpellsAndEffectsActions::GetAttackValueAfterResistances(ValueOfAttack, Effects, Resistances, ContainedElements);
				if (HP - (ValueOfAttackWithResistances - ValueOfArmor / 10) < 0)
					HP = 0;
				else
					HP -= (ValueOfAttackWithResistances - ValueOfArmor / 10);
				if (IsValid(EnemyHealthBarWidget))
					EnemyHealthBarWidget->HP = HP;
				TextForCombatFloatingInformationActor.AppendInt(ValueOfAttackWithResistances);
				CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
			}
			UCombatNPCAnimInstance* AnimInstance = Cast<UCombatNPCAnimInstance>(GetMesh()->GetAnimInstance());
			if (IsValid(AnimInstance)) {
				if (HP <= 0)
					AnimInstance->SetCombatEnemyNPCIsDead(true);
				else
					AnimInstance->SetCombatEnemyNPCGotHit(true);
			}
		}
}

UEnemyHealthBarWidget* ACombatEnemyNPC::GetEnemyHealthBarWidget() const
{
	return EnemyHealthBarWidget;
}

TArray<FElementAndItsPercentageStruct> ACombatEnemyNPC::GetResistances() const
{
	return Resistances;
}

TArray<FElementAndItsPercentageStruct> ACombatEnemyNPC::GetMeleeWeaponElements() const
{
	return MeleeWeaponElements;
}

TArray<FElementAndItsPercentageStruct> ACombatEnemyNPC::GetRangeWeaponElements() const
{
	return RangeWeaponElements;
}
