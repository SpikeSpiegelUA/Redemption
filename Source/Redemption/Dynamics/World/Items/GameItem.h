// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameItem.generated.h"

UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDamageKind : uint8
{
	None = 0x00000000,
	Melee = 0x00000001,
	Range = 0x00000002,
	Earth = 0x00000004,
	Buff = 0x00000008,
	Debuff = 0x00000010,
	Wind = 0x00000020,
	Fire = 0x00000040,

	MELEE UMETA(DisplayName = "Melee"),
	RANGE UMETA(DisplayName = "Range"),
	EARTH UMETA(DisplayName = "Earth"),
	WIND UMETA(DisplayName = "Wind"),
	FIRE UMETA(DisplayName = "Fire"),
	LIGHTNING UMETA(DisplayName = "Lightning"),
	BUFF UMETA(DisplayName = "Buff"),
	DEBUFF UMETA(DisplayName = "Debuff"),
};

ENUM_CLASS_FLAGS(EDamageKind)

UENUM(BlueprintType)
enum class EItemType :uint8
{
	ASSAULT UMETA(DisplayName = "Assault"),
	RESTORATION UMETA(DisplayName = "Restoration"),
	BUFF UMETA(DisplayName = "Buff"),
	DEBUFF UMETA(DisplayName = "Debuff"),
	MISCELLANEOUS UMETA(DisplayName = "Miscellaneous"),
	EQUIPMENT UMETA(DisplayName = "Equipment"),
};

UCLASS()
class AGameItem : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "EDamageKind"), Category = "General Information")
		int32 DamageKindBitmaskCode;
public:	
	// Sets default values for this actor's properties
	AGameItem();

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		FName Name = "";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		EItemType Type{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		int Cost{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		FText Description{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName GetItemName() const;
	EItemType GetItemType() const;
	int GetCost() const;
	FText GetDescription() const;
};
