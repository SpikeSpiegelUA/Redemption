// Fill out your copyright notice in the Description page of Project Settings.


#include "..\Dynamics\Gameplay\Managers\EffectsSpellsAndSkillsManager.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEffectsSpellsAndSkillsManager::AEffectsSpellsAndSkillsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEffectsSpellsAndSkillsManager::BeginPlay()
{
	Super::BeginPlay();
	if (auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		RedemptionGameModeBase->SetEffectsSpellsAndSkillsManager(this);
}

// Called every frame
void AEffectsSpellsAndSkillsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UDataTable* AEffectsSpellsAndSkillsManager::GetEffectsDataTable() const
{
	return EffectsDataTable;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetFireMainElementSpellObjectClass() const
{
	return FireMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetBloodMainElementSpellObjectClass() const
{
	return BloodMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetDarkMainElementSpellObjectClass() const
{
	return DarkMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetEarthMainElementSpellObjectClass() const
{
	return EarthMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetHolyMainElementSpellObjectClass() const
{
	return HolyMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetLightningMainElementSpellObjectClass() const
{
	return LightningMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetWaterMainElementSpellObjectClass() const
{
	return WaterMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetWindMainElementSpellObjectClass() const
{
	return WindMainElementSpellObjectClass;
}

TSubclassOf<ASpellObject> AEffectsSpellsAndSkillsManager::GetMultielementalMainElementSpellObjectClass() const
{
	return MultiElementalMainElementSpellObjectClass;
}

UTexture* AEffectsSpellsAndSkillsManager::GetFireElementIcon() const
{
	return FireElementIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetWaterElementIcon() const
{
	return WaterElementIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetWindElementIcon() const
{
	return WindElementIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetEarthElementIcon() const
{
	return EarthElementIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetLightningElementIcon() const
{
	return LightningElementIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetBloodElementIcon() const
{
	return BloodElementIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetHolyElementIcon() const
{
	return HolyElementIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetDarkElementIcon() const
{
	return DarkElementIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetMultielementalIcon() const
{
	return MultielementalIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetAssaultSpellTypeIcon() const
{
	return AssaultSpellTypeIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetDebuffSpellTypeIcon() const
{
	return DebuffSpellTypeIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetRestorationSpellTypeIcon() const
{
	return RestorationSpellTypeIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetBuffSpellTypeIcon() const
{
	return BuffSpellTypeIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetArmorEffectAreaIcon() const
{
	return ArmorEffectAreaIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetEvasionEffectAreaIcon() const
{
	return EvasionEffectAreaIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetSingleSpellRangeIcon() const
{
	return SingleSpellRangeIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetNeighborsSpellRangeIcon() const
{
	return NeighborsSpellRangeIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetEveryoneSpellRangeIcon() const
{
	return EveryoneSpellRangeIcon;
}

UTexture* AEffectsSpellsAndSkillsManager::GetSpellTypeImageTexture(ESpellType SpellType) const
{
	switch (SpellType) {
	case ESpellType::ASSAULT:
		return AssaultSpellTypeIcon;
	case ESpellType::DEBUFF:
		return DebuffSpellTypeIcon;
	case ESpellType::RESTORATION:
		return RestorationSpellTypeIcon;
	case ESpellType::BUFF:
		return BuffSpellTypeIcon;
	default:
		return nullptr;
	}
}

UTexture* AEffectsSpellsAndSkillsManager::GetMainSpellElementImageTexture(ESpellElements MainSpellElement) const
{
	switch (MainSpellElement) {
	case ESpellElements::WATER:
		return WaterElementIcon;
	case ESpellElements::EARTH:
		return EarthElementIcon;
	case ESpellElements::DARK:
		return DarkElementIcon;
	case ESpellElements::LIGHTNING:
		return LightningElementIcon;
	case ESpellElements::HOLY:
		return HolyElementIcon;
	case ESpellElements::WIND:
		return WindElementIcon;
	case ESpellElements::FIRE:
		return FireElementIcon;
	case ESpellElements::BLOOD:
		return BloodElementIcon;
	case ESpellElements::MULTIELEMENTAL:
		return MultielementalIcon;
	default:
		return nullptr;
	}
}

UTexture* AEffectsSpellsAndSkillsManager::GetEffectAreaImageTexture(EEffectArea EffectArea) const
{
	switch (EffectArea) {
		case EEffectArea::DAMAGE:
			return AssaultSpellTypeIcon;
		case EEffectArea::ARMOR:
			return ArmorEffectAreaIcon;
		case EEffectArea::EVASION:
			return EvasionEffectAreaIcon;
		case EEffectArea::FIRERESISTANCE:
			return FireElementIcon;
		case EEffectArea::WATERRESISTANCE:
			return WaterElementIcon;
		case EEffectArea::WINDRESISTANCE:
			return WindElementIcon;
		case EEffectArea::EARTHRESISTANCE:
			return EarthElementIcon;
		case EEffectArea::LIGHTNINGRESISTANCE:
			return LightningElementIcon;
		case EEffectArea::HOLYRESISTANCE:
			return HolyElementIcon;
		case EEffectArea::DARKRESISTANCE:
			return DarkElementIcon;
		case EEffectArea::BLOODRESISTANCE:
			return BloodElementIcon;
		case EEffectArea::MULTIELEMENTALRESISTANCE:
			return MultielementalIcon;
		default:
			return nullptr;
	}
}

UTexture* AEffectsSpellsAndSkillsManager::GetEffectTypeImageTexture(EEffectType EffectType) const
{
	switch (EffectType) {
	case EEffectType::BUFF:
		return BuffSpellTypeIcon;
	case EEffectType::PLAINBUFF:
		return BuffSpellTypeIcon;
	case EEffectType::PERCENTBUFF:
		return BuffSpellTypeIcon;
	case EEffectType::DEBUFF:
		return DebuffSpellTypeIcon;
	case EEffectType::PLAINDEBUFF:
		return DebuffSpellTypeIcon;
	case EEffectType::PERCENTDEBUFF:
		return DebuffSpellTypeIcon;
	default:
		return nullptr;
	}
}

