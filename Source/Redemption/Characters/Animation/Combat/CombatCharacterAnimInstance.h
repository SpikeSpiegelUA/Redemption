// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "..\Characters\Combat\CombatNPC.h"
#include "CombatCharacterAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum ECombatCharacterAnimationState
{
	CombatCharacterNone = 0x0000,
	CombatCharacterIsAttacking = 0x0001,
	CombatCharacterIsDead = 0x0002,
	CombatCharacterGotHit = 0x0004,
	CombatCharacterIsAttackingWithMagic = 0x0008,
	CombatCharacterIsAiming = 0x0100
};

ENUM_CLASS_FLAGS(ECombatCharacterAnimationState)

UCLASS()
class REDEMPTION_API UCombatCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UCombatCharacterAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UPROPERTY(BlueprintReadOnly, Category = "Animation Properties")
		class ACombatNPC* CombatNPCReference;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		float Speed{};
	UFUNCTION(BlueprintCallable, Category = "Animations")
		virtual void UpdateAnimProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "ECombatCharacterAnimationState"), Category = "General Information")
		int32 CombatCharacterAnimationStateBitmaskCode {};
public:
	void ToggleCombatCharacterIsAttacking(bool Value);
	void ToggleCombatCharacterIsDead(bool Value);
	void ToggleCombatCharacterGotHit(bool Value);
	void ToggleCombatCharacterIsAttackingWithMagic(bool Value);
	void ToggleCombatCharacterIsAiming(bool Value);

	bool GetCombatCharacterIsDead() const;
	bool GetCombatCharacterGotHit() const;
	bool GetCombatCharacterIsAiming();
};
