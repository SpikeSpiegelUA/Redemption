// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\SkillsSpellsAndEffectsActions.h"

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

int ACombatNPC::GetHP() const
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

void ACombatNPC::SetStartLocation(AActor* const& NewLocation)
{
	StartLocation = NewLocation;
}
