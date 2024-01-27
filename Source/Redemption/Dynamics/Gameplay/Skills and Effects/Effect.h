// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Effect.generated.h"

UENUM(BlueprintType)
enum class EEffectArea :uint8
{
	NONE UMETA(Hidden),
	DAMAGE UMETA(DisplayName = "Damage"),
	ARMOR UMETA(DisplayName = "Armor"),
	EVASION UMETA(DisplayName = "Evasion"),
	FIRERESISTANCE UMETA(DisplayName = "Fire resistance"),
	WATERRESISTANCE UMETA(DisplayName = "Water resistance"),
	WINDRESISTANCE UMETA(DisplayName = "Wind resistance"),
	EARTHRESISTANCE UMETA(DisplayName = "Earth resistance"),
	LIGHTNINGRESISTANCE UMETA(DisplayName = "Lightning resistance"),
	HOLYRESISTANCE UMETA(DisplayName = "Holy resistance"),
	DARKRESISTANCE UMETA(DisplayName = "Dark resistance"),
	BLOODRESISTANCE UMETA(DisplayName = "Blood resistance"),
	MULTIELEMENTALRESISTANCE UMETA(DisplayName = "Multielemental resistance"),
	CRUSHINGRESISTANCE UMETA(DisplayName = "Crushing resistance"),
	SLASHINGRESISTANCE UMETA(DisplayName = "Slashing resistance"),
	PIERCINGRESISTANCE UMETA(DisplayName = "Piercing resistance"),
};

UENUM(BlueprintType)
enum class ESpellElements :uint8
{
	NONE UMETA(Hidden),
	FIRE UMETA(DisplayName = "Fire"),
	WATER UMETA(DisplayName = "Water"),
	WIND UMETA(DisplayName = "Wind"),
	EARTH UMETA(DisplayName = "Earth"),
	LIGHTNING UMETA(DisplayName = "Lightning"),
	HOLY UMETA(DisplayName = "Holy"),
	DARK UMETA(DisplayName = "Dark"),
	BLOOD UMETA(DisplayName = "Blood"),

	//Need this for some logic. Basically spell/weapon has different elements with the same count.
	MULTIELEMENTAL UMETA(DisplayName = "Multielemental")
};
ENUM_RANGE_BY_FIRST_AND_LAST(ESpellElements, ESpellElements::FIRE, ESpellElements::MULTIELEMENTAL);

UENUM(BlueprintType)
enum class EEffectType :uint8
{
	BUFF UMETA(DisplayName = "Buff"),
	DEBUFF UMETA(DisplayName = "Debuff"),
	PLAINBUFF UMETA(DisplayName = "Plain Buff"),
	PLAINDEBUFF UMETA(DisplayName = "Plain Debuff"),
	TURNSKIP UMETA(DisplayName = "Turn skip"),
	TURNSTARTDAMAGE UMETA(DisplayName = "Turn start damage"),
	DIZZINESS UMETA(DisplayName = "Dizziness")
};

UCLASS()
class AEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		EEffectArea AreaOfEffect {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		EEffectType TypeOfEffect {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		FText EffectName {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		FText EffectDescription{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		int EffectStat{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		int Duration{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EEffectArea GetEffectArea() const;
	EEffectType GetEffectType() const;
	const FText& GetEffectName() const;
	const FText& GetEffectDescription() const;
	int GetEffectStat() const;
	int GetDuration() const;

	void SetEffectArea(EEffectArea NewAreaOfEffect);
	void SetEffectType(EEffectType NewTypeOfEffect);
	void SetEffectName(const FText& NewEffectName);
	void SetEffectStat(int NewEffectStat);
	void SetDuration(int NewDuration);
	void SetEffectDescription(const FText& NewEffectDescription);

	int8 CurrentDuration = 0;

	void CopyEffect(const AEffect* const EffectToCopy);
};
