// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Effect.h"
#include "Spell.generated.h"

UENUM(BlueprintType)
enum class ESpellType :uint8
{
	ASSAULT UMETA(DisplayName = "Assault"),
	BUFF UMETA(DisplayName = "Buff"),
	RESTORATION UMETA(DisplayName = "Restoration"),
	DEBUFF UMETA(DisplayName = "Debuff"),
};

UCLASS()
class REDEMPTION_API ASpell : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		FName SpellName {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		TArray<ESpellElement> RequiredElements;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		int ManaCost{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		ESpellType TypeOfSpell {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "General Information")
		FText Description {};

public:	
	// Sets default values for this actor's properties
	ASpell();

	FName GetSpellName() const;
	TArray<ESpellElement> GetRequiredElements() const;
	int GetManaCost() const;
	ESpellType GetTypeOfSpell() const;
	FText GetDescription() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};