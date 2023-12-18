// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "Spell.generated.h"

UENUM(BlueprintType)
enum class ESpellType :uint8
{
	NONE UMETA(Hidden),
	ASSAULT UMETA(DisplayName = "Assault"),
	BUFF UMETA(DisplayName = "Buff"),
	RESTORATION UMETA(DisplayName = "Restoration"),
	DEBUFF UMETA(DisplayName = "Debuff")
};

UENUM(BlueprintType)
enum class ESpellCostType :uint8
{
	NONE UMETA(Hidden),
	HEALTH UMETA(DisplayName = "Health"),
	MANA UMETA(DisplayName = "Mana")
};

UENUM(BlueprintType)
enum class ESpellRange :uint8
{
	NONE UMETA(Hidden),
	SINGLE UMETA(DisplayName = "Single"),
	NEIGHBORS UMETA(DisplayName = "Neighbors"),
	EVERYONE UMETA(DisplayName = "Everyone")
};

UENUM(BlueprintType)
enum class EBuffDebuffType :uint8
{
	NONE UMETA(Hidden),
	DAMAGE UMETA(DisplayName = "Attack"),
	ARMOR UMETA(DisplayName = "Armor"),
	EVASION UMETA(DisplayName = "Agility"),
	ELEMENTALRESISTANCE UMETA(DisplayName = "Elemental Resistance"),
	PHYSICALRESISTANCE UMETA(DisplayName = "Physical Resistance")
};

UCLASS()
class REDEMPTION_API ASpell : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		FText SpellName {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		 ESpellCostType SpellCostType{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		int Cost{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		ESpellType TypeOfSpell {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		ESpellRange SpellRange{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		FText Description {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		TArray<ESpellElements> SpellElements {};

public:	
	// Sets default values for this actor's properties
	ASpell();

	FText GetSpellName() const;
	ESpellCostType GetSpellCostType() const;
	int GetCost() const;
	ESpellType GetTypeOfSpell() const;
	ESpellRange GetSpellRange() const;
	FText GetDescription() const;
	TArray<ESpellElements> GetSpellElements() const;

	void SetSpellName(const FText& NewSpellName);
	void SetCost(const int NewCost);
	void SetSpellCostType(const ESpellCostType NewSpellCostType);
	void SetTypeOfSpell(const ESpellType NewTypeOfSpell);
	void SetDescription(const FText& NewDescription);
	void SetSpellElements(const TArray<ESpellElements>& NewSpellElements);
	void SetSpellRange(const ESpellRange NewSpellRange);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
