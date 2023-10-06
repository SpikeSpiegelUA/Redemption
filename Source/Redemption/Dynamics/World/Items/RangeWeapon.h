// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "RangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ARangeWeapon : public AWeaponItem
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		int Ammo{};

public:
	int GetAmmo() const;
};
