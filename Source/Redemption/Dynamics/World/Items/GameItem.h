// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "GameItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8 
{
	NONE UMETA(Hidden),
	ASSAULT UMETA(DisplayName = "Assault"),
	RESTORATION UMETA(DisplayName = "Restoration"),
	BUFF UMETA(DisplayName = "Buff"),
	DEBUFF UMETA(DisplayName = "Debuff"),
	MISCELLANEOUS UMETA(DisplayName = "Miscellaneous"),
	WEAPON UMETA(DisplayName = "Weapon"),
	ARMOR UMETA(DisplayName = "Armor")
};

UENUM(BlueprintType)
enum class EItemRange : uint8
{
	NONE UMETA(Hidden),
	SINGLE UMETA(DisplayName = "Single"),
	NEIGHBORS UMETA(DisplayName = "Neighbors"),
	EVERYONE UMETA(DisplayName = "Everyone")
};

UCLASS()
class AGameItem : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		FName Name = "";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		int Cost{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		FText Description{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		EItemType ItemType{};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		EItemRange ItemRange{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Sets default values for this actor's properties
	AGameItem();

	FName GetItemName() const;
	int GetCost() const;
	FText GetDescription() const;
	EItemType GetItemType() const;
	EItemRange GetItemRange() const;
};
