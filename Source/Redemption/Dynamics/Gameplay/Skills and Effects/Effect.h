// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Effect.generated.h"

UENUM(BlueprintType)
enum class EffectArea :uint8
{
	DAMAGE UMETA(DisplayName = "Damage"),
	ARMOR UMETA(DisplayName = "Armor"),
	EVASION UMETA(DisplayName = "Evasion"),
};

UENUM(BlueprintType)
enum class SpellElement :uint8
{
	FIRE UMETA(DisplayName = "Fire"),
	WATER UMETA(DisplayName = "Water"),
	WIND UMETA(DisplayName = "Wind"),
	EARTH UMETA(DisplayName = "Wind"),
	LIGHTNING UMETA(DisplayName = "Lightning"),
	HOLY UMETA(DisplayName = "Holy"),
	DARK UMETA(DisplayName = "Dark"),
	BLOOD UMETA(DisplayName = "Blood"),
};

UENUM(BlueprintType)
enum class EffectType :uint8
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
		EffectArea AreaOfEffect {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		EffectType TypeOfEffect {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		FName EffectName {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		int EffectStat{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
		int Duration{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EffectArea GetAreaOfEffect() const;
	EffectType GetTypeOfEffect() const;
	FName GetEffectName() const;
	int GetEffectStat() const;
	int GetDuration() const;

	int8 CurrentDuration = 0;
};
