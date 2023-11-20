// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\ElementsActions.h"

ACombatNPC::ACombatNPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Create ComponentWidget for FloatingHealthBar
	FloatingHealthBarComponentWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	FloatingHealthBarComponentWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingHealthBarComponentWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACombatNPC::BeginPlay()
{
	Super::BeginPlay();
	//Set up properties for FloatingHealthBar
	FloatingHealthBarWidget = Cast<UFloatingHealthBarWidget>(FloatingHealthBarComponentWidget->GetWidget());
	if (IsValid(FloatingHealthBarWidget)) {
		FloatingHealthBarWidget->HP = CurrentHP;
		FloatingHealthBarWidget->MaxHP = CurrentHP;
	}
	SkillsSpellsAndEffectsActions::InitializeElementalResistances(Resistances);
}
// Called every frame
void ACombatNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatNPC::GetHitWithBuffOrDebuff_Implementation(const TArray<class AEffect*>& HitEffects, const TArray<FElementAndItsPercentageStruct>& ContainedElements)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager)) {
			ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(),
				GetActorLocation(), GetActorRotation());
			FString TextForCombatFloatingInformationActor = FString();
			uint8 EvasionRandomNumber = FMath::RandRange(0, 100);
			int ChanceOfEvasion = SkillsSpellsAndEffectsActions::GetBuffOrDebuffEvasionChanceAfterResistances(SkillsSpellsAndEffectsActions::GetValueAfterEffects(EvasionChance + Agility * 2,
				Effects, EEffectArea::EVASION), Effects, Resistances, ContainedElements);
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

void ACombatNPC::GetHit_Implementation(int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements, bool ForcedMiss)
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
			UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(GetMesh()->GetAnimInstance());
			if (IsValid(AnimInstance)) {
				if (CurrentHP <= 0)
					AnimInstance->ToggleCombatCharacterIsDead(true);
				else if (UCombatAlliesAnimInstance* CombatAlliesAnimInstance = Cast<UCombatAlliesAnimInstance>(GetMesh()->GetAnimInstance()); IsValid(CombatAlliesAnimInstance))
					if (CurrentHP > 0 && !CombatAlliesAnimInstance->GetCombatAlliesIsBlocking())
						AnimInstance->ToggleCombatCharacterGotHit(true);
			}
		}
}

UFloatingHealthBarWidget* ACombatNPC::GetFloatingHealthBarWidget() const
{
	return FloatingHealthBarWidget;
}

TArray<FElementAndItsPercentageStruct> ACombatNPC::GetResistances() const
{
	return Resistances;
}

TArray<FElementAndItsPercentageStruct> ACombatNPC::GetMeleeWeaponElements() const
{
	return MeleeWeaponElements;
}

TArray<FElementAndItsPercentageStruct> ACombatNPC::GetRangeWeaponElements() const
{
	return RangeWeaponElements;
}

TArray<AEffect*> ACombatNPC::GetEffects() const
{
	return Effects;
}

int ACombatNPC::GetCurrentHP() const
{
	return CurrentHP;
}

int ACombatNPC::GetMeleeAttackValue() const
{
	return MeleeAttackValue;
}

int ACombatNPC::GetRangeAttackValue() const
{
	return RangeAttackValue;
}

int ACombatNPC::GetRangeAmmo() const
{
	return RangeAmmo;
}

AActor* ACombatNPC::GetStartLocation() const
{
	return StartLocation;
}

TSubclassOf<ASmartObject> ACombatNPC::GetAIClass() const
{
	return AIClass;
}

float ACombatNPC::GetHealthPercentage()
{
	return CurrentHP / MaxHP;
}

float ACombatNPC::GetManaPercentage()
{
	return CurrentMana/MaxMana;
}

const TArray<TSubclassOf<ASpell>>& ACombatNPC::GetAvailableSkills() const
{
	return AvailableSkills;
}

void ACombatNPC::SetRangeAmmo(int8 NewRangeAmmo)
{
	RangeAmmo = NewRangeAmmo;
}

void ACombatNPC::SetStartLocation(const AActor* const NewLocation)
{
	StartLocation = const_cast<AActor*>(NewLocation);
}


