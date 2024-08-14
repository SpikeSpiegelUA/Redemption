// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Redemption/Dynamics/Gameplay/Skills and Effects/Effects/Effect.h"
#include "Perk.generated.h"

UENUM(BlueprintType)
enum class EPerkValueType :uint8
{
	NONE UMETA(Hidden),
	STANDARD UMETA(DisplayName = "Standard"),
	PLAIN UMETA(DisplayName = "Plain"),
	PERCENTAGE UMETA(DisplayName = "Percentage")
};

UCLASS()
class REDEMPTION_API APerk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information")
	FName PerkName{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information")
	FText PerkDescription{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information")
	EEffectArea EffectArea{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information")
	int PerkPrice{};
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	const EEffectArea GetEffectArea() const;
	const FName& GetPerkName() const;
	const FText& GetPerkDescription() const;
	const int GetPerkPrice() const;
};
