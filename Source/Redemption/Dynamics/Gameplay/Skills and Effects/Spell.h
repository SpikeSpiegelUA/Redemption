// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "Spell.generated.h"

UENUM(BlueprintType)
enum class ESpellType :uint8
{
	NONE UMETA(Hidden),
	ASSAULT UMETA(DisplayName = "Assault"),
	BUFF UMETA(DisplayName = "Buff"),
	RESTORATION UMETA(DisplayName = "Restoration"),
	DEBUFF UMETA(DisplayName = "Debuff"),
};

UENUM(BlueprintType)
enum class EBuffDebuffType :uint8
{
	DAMAGE UMETA(DisplayName = "Attack"),
	ARMOR UMETA(DisplayName = "Armor"),
	EVASION UMETA(DisplayName = "Agility"),
	ELEMENTALRESISTANCE UMETA(DisplayName = "Elemental Resistance"),
	PHYSICALRESISTANCE UMETA(DisplayName = "Physical Resistance"),
};

UCLASS()
class REDEMPTION_API ASpell : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		FText SpellName {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		int ManaCost{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		ESpellType TypeOfSpell {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		FText Description {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		TArray<ESpellElements> SpellElements {};

public:	
	// Sets default values for this actor's properties
	ASpell();

	FText GetSpellName() const;
	int GetManaCost() const;
	ESpellType GetTypeOfSpell() const;
	FText GetDescription() const;
	TArray<ESpellElements> GetSpellElements() const;

	void SetSpellName(FText& NewSpellName);
	void SetSpellName(FText NewSpellName);
	void SetManaCost(int NewManaCost);
	void SetTypeOfSpell(ESpellType NewTypeOfSpell);
	void SetDescription(FText& NewDescription);
	void SetDescription(FText NewDescription);
	void SetSpellElements(const TArray<ESpellElements>& NewSpellElements);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
