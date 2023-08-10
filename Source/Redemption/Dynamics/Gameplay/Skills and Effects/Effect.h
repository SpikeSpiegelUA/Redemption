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
	FIRERESISTANCE UMETA(DisplayName = "Fire Resistance"),
	WATERRESISTANCE UMETA(DisplayName = "Water Resistance"),
	WINDRESISTANCE UMETA(DisplayName = "Wind Resistance"),
	EARTHRESISTANCE UMETA(DisplayName = "Earth Resistance"),
	LIGHTNINGRESISTANCE UMETA(DisplayName = "Lightning Resistance"),
	HOLYRESISTANCE UMETA(DisplayName = "Holy Resistance"),
	DARKRESISTANCE UMETA(DisplayName = "Dark Resistance"),
	BLOODRESISTANCE UMETA(DisplayName = "Blood Resistance"),
	MULTIELEMENTALRESISTANCE UMETA(DisplayName = "Multielemental Resistance"),
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

	//Need this for some logic. Basically spell/weapon has different elements with the same count. Affects only spell info and spell's spell object. Isn't a part of the combat design.
	MULTIELEMENTAL UMETA(DisplayName = "Multielemental"),
};

UENUM(BlueprintType)
enum class EEffectType :uint8
{
	BUFF UMETA(DisplayName = "Buff"),
	DEBUFF UMETA(DisplayName = "Debuff"),
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
		int EffectStat{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		int Duration{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EEffectArea GetAreaOfEffect() const;
	EEffectType GetTypeOfEffect() const;
	FText GetEffectName() const;
	int GetEffectStat() const;
	int GetDuration() const;

	void SetAreaOfEffect(EEffectArea NewAreaOfEffect);
	void SetTypeOfEffect(EEffectType NewTypeOfEffect);
	void SetEffectName(const FText& NewEffectName);
	void SetEffectStat(int NewEffectStat);
	void SetDuration(int NewDuration);

	int8 CurrentDuration = 0;
};
