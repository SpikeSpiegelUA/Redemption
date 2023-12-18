// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatAlliesAnimInstance.h"
#include "CombatPlayerCharacterAnimInstanc.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum ECombatPlayerCharacterAnimationState
{
	CombatPlayerCharacterNone = 0x0000,
};

ENUM_CLASS_FLAGS(ECombatPlayerCharacterAnimationState)

UCLASS()
class REDEMPTION_API UCombatPlayerCharacterAnimInstanc : public UCombatAlliesAnimInstance
{
	GENERATED_BODY()

public:
	UCombatPlayerCharacterAnimInstanc();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void UpdateAnimProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "/Script/Redemption.ECombatPlayerCharacterAnimationState"), Category = "General Information")
		int32 CombatPlayerCharacterAnimationStateBitmaskCode {};
};
