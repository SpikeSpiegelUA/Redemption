// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatCharacterAnimInstance.h"
#include "CombatEnemyNPCAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum ECombatEnemyNPCAnimationState
{
	CombatEnemyNPCNone = 0x0000,
};

ENUM_CLASS_FLAGS(ECombatEnemyNPCAnimationState)

UCLASS()
class REDEMPTION_API UCombatEnemyNPCAnimInstance : public UCombatCharacterAnimInstance
{
	GENERATED_BODY()
public:
	UCombatEnemyNPCAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void UpdateAnimProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "ECombatEnemyNPCAnimationState"), Category = "General Information")
		int32 CombatEnemyNPCAnimationStateBitmaskCode {};
};
