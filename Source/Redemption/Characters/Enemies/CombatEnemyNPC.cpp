// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatEnemyNPC.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\CombatNPCAnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\EffectWithPlainModifier.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ElementsActions.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Combat\CombatFloatingInformationActor.h"
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

void ACombatEnemyNPC::GetHitWithBuffOrDebuff_Implementation(class AEffect* const& Effect) 
{
	Effects.Add(Effect);
}

void ACombatEnemyNPC::GetHit_Implementation(int ValueOfAttack, const TArray<ESpellElements>& ContainedElements)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager)) {
			int ValueOfArmor = ArmorValue;
			int ValueOfArmorBeforeEffects = ValueOfArmor;
			for (AEffect* Effect : Effects) {
				if (IsValid(Effect) && Effect->GetAreaOfEffect() == EEffectArea::ARMOR) {
					if (AEffectWithPlainModifier* EffectWithPlainModifier = Cast<AEffectWithPlainModifier>(Effect); IsValid(EffectWithPlainModifier)) {
						if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
							ValueOfArmor += ValueOfArmorBeforeEffects + Effect->GetEffectStat();
						else
							ValueOfArmor += ValueOfArmorBeforeEffects - Effect->GetEffectStat();
					}
					else {
						if (Effect->GetTypeOfEffect() == EEffectType::BUFF)
							ValueOfArmor += ValueOfArmorBeforeEffects * (Effect->GetEffectStat() - 1);
						else
							ValueOfArmor -= ValueOfArmorBeforeEffects / Effect->GetEffectStat();
					}
				}
			}
			int ValueOfAttackiWithResistances = ValueOfAttack;
			TArray<FElementAndItsPercentageStruct> ElementsAndTheirPercents = ElementsActions::FindContainedElements<ESpellElements>(ContainedElements);
			for (FElementAndItsPercentageStruct ElementPercent : ElementsAndTheirPercents)
				for (int8 i = 0; i < MagicResistances.Num(); i++)
					if (MagicResistances[i] == ElementPercent.GetElement()) {
						int ElementPercentOfValueOfAttack = ValueOfAttack * ElementPercent.GetPercent() / 100;
						ValueOfAttackiWithResistances -= ElementPercentOfValueOfAttack * MagicResistancesPercent[i] / 100;
					}
			if (HP - (ValueOfAttackiWithResistances - ValueOfArmor / 10) < 0)
				HP = 0;
			else
				HP -= (ValueOfAttackiWithResistances - ValueOfArmor / 10);
			if (IsValid(EnemyHealthBarWidget))
				EnemyHealthBarWidget->HP = HP;
			UCombatNPCAnimInstance* AnimInstance = Cast<UCombatNPCAnimInstance>(GetMesh()->GetAnimInstance());
			if (IsValid(AnimInstance)) {
				if (HP <= 0)
					AnimInstance->SetCombatEnemyNPCIsDead(true);
				else
					AnimInstance->SetCombatEnemyNPCGotHit(true);
			}
			ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), GetActorLocation(), GetActorRotation());
		}
}

UEnemyHealthBarWidget* ACombatEnemyNPC::GetEnemyHealthBarWidget() const
{
	return EnemyHealthBarWidget;
}
