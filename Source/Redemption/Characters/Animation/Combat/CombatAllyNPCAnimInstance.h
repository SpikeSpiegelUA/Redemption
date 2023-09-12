// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatAlliesAnimInstance.h"
#include "CombatAllyNPCAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum ECombatAllyNPCAnimationState
{
	CombatAllyNPCNone = 0x0000,
};

ENUM_CLASS_FLAGS(ECombatAllyNPCAnimationState)

UCLASS()
class REDEMPTION_API UCombatAllyNPCAnimInstance : public UCombatAlliesAnimInstance
{
	GENERATED_BODY()
public:
	UCombatAllyNPCAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void UpdateAnimProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "ECombatAllyNPCAnimationState"), Category = "General Information")
		int32 CombatAllyNPCAnimationStateBitmaskCode {};
};

