// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAllies.h"
#include "..\Characters\AI Controllers\Combat\CombatAlliesAIController.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Characters\Animation\Combat\CombatAlliesAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\UI\HUD\FloatingManaBarWidget.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ACombatAllies::ACombatAllies()
{
	//Create ComponentWidget for FloatingManaBar
	FloatingManaBarComponentWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Mana Bar"));
	FloatingManaBarComponentWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingManaBarComponentWidget->SetupAttachment(RootComponent);

	InitializeSkillsProgress();
}

void ACombatAllies::BeginPlay()
{
	Super::BeginPlay();
	//Set up properties for FloatingManaBar
	FloatingManaBarWidget = Cast<UFloatingManaBarWidget>(FloatingManaBarComponentWidget->GetWidget());
	if (IsValid(FloatingManaBarWidget)) {
		FloatingManaBarWidget->Mana = CurrentMana;
		FloatingManaBarWidget->MaxMana = MaxMana;
	}
}

void ACombatAllies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatAllies::StartMovingToEnemy()
{
	IsMovingToAttackEnemy = true;
	if (ACombatAlliesAIController* CombatAllyAIController = Cast<ACombatAlliesAIController>(GetController()); IsValid(CombatAllyAIController))
		CombatAllyAIController->MoveToActor(Target, 85.f);
}

void ACombatAllies::StartMovingToStartLocation()
{
	IsMovingToStartPosition = true;
	if(ACombatAlliesAIController* CombatAlliesAIController = Cast<ACombatAlliesAIController>(GetController()); IsValid(CombatAlliesAIController))
		CombatAlliesAIController->MoveToActor(StartLocation);
	
}

void ACombatAllies::ResetSkillsLeveledUp()
{
	SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpMelee, false);
	SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpRange, false);
	SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpAssaultSpells, false);
	SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpDebuffSpells, false);
	SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpRestorationSpells, false);
	SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpBuffSpells, false);
	SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpDefend, false);
	SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpPersuasion, false);
}

void ACombatAllies::InitializeSkillsProgress()
{
	SkillsProgressMap.Add(ECharacterSkills::MELEE, 0);
	SkillsProgressMap.Add(ECharacterSkills::RANGE, 0);
	SkillsProgressMap.Add(ECharacterSkills::ASSAULTSPELLS, 0);
	SkillsProgressMap.Add(ECharacterSkills::DEBUFFSPELLS, 0);
	SkillsProgressMap.Add(ECharacterSkills::RESTORATIONSPELLS, 0);
	SkillsProgressMap.Add(ECharacterSkills::BUFFSPELLS, 0);
	SkillsProgressMap.Add(ECharacterSkills::DEFEND, 0);
	SkillsProgressMap.Add(ECharacterSkills::PERSUASION, 0);
}

const int16 ACombatAllies::GetSkillsProgress(const ECharacterSkills SkillToGet) const
{
	return *SkillsProgressMap.Find(SkillToGet);
}

void ACombatAllies::SetSkillsProgress(const ECharacterSkills SkillToGet, const int16 NewValue)
{
	SkillsProgressMap.Emplace(SkillToGet, NewValue);
}

void ACombatAllies::SetSkillsLeveledUp(const ESkillsLeveledUp SkillToSet, const bool Value)
{
	if (Value)
		BitmapsActions::SetBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(SkillToSet));
	else
		BitmapsActions::ClearBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(SkillToSet));
}

void ACombatAllies::AddSkillsProgress(ECharacterSkills SkillToAddTo, const int16 ValueToAdd)
{
	SkillsProgressMap.Emplace(SkillToAddTo, *SkillsProgressMap.Find(SkillToAddTo) + ValueToAdd);
}

void ACombatAllies::AddSkills(const ECharacterSkills SkillToAddTo, const int16 ValueToAdd)
{
	SkillsMap.Emplace(SkillToAddTo, *SkillsMap.Find(SkillToAddTo) + ValueToAdd);
}

const TMap<ECharacterSkills, int>& ACombatAllies::GetSkillsMap() const
{
	return SkillsMap;
}

const TMap<ECharacterSkills, int>& ACombatAllies::GetSkillsProgressMap() const
{
	return SkillsProgressMap;
}

const TArray<FText>& ACombatAllies::GetPerksCategoryNames() const
{
	return PerksCategoryNames;
}

const TArray<TSubclassOf<APerk>>& ACombatAllies::GetAvailablePerks() const
{
	return AvailablePerks;
}

const bool ACombatAllies::GetSkillsLeveledUp(const ESkillsLeveledUp SkillToGet) const
{
	return BitmapsActions::TestBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(SkillToGet));
}

const bool ACombatAllies::WasSkillLeveledUp() const
{
	if (BitmapsActions::TestBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(ESkillsLeveledUp::SkillsLeveledUpMelee)))
		return true;
	if (BitmapsActions::TestBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(ESkillsLeveledUp::SkillsLeveledUpRange)))
		return true;
	if (BitmapsActions::TestBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(ESkillsLeveledUp::SkillsLeveledUpDefend)))
		return true;
	if (BitmapsActions::TestBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(ESkillsLeveledUp::SkillsLeveledUpAssaultSpells)))
		return true;
	if (BitmapsActions::TestBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(ESkillsLeveledUp::SkillsLeveledUpBuffSpells)))
		return true;
	if (BitmapsActions::TestBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(ESkillsLeveledUp::SkillsLeveledUpRestorationSpells)))
		return true;
	if (BitmapsActions::TestBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(ESkillsLeveledUp::SkillsLeveledUpDebuffSpells)))
		return true;
	if (BitmapsActions::TestBit(SkillsLeveledUpBitmaskCode, static_cast<int32>(ESkillsLeveledUp::SkillsLeveledUpPersuasion)))
		return true;
	return false;
}

const UTexture* ACombatAllies::GetCharacterPortrait() const
{
	return CharacterPortrait;
}

UFloatingManaBarWidget* ACombatAllies::GetFloatingManaBarWidget() const
{
	return FloatingManaBarWidget;
}
