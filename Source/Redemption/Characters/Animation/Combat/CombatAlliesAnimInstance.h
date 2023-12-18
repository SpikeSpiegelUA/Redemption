// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatCharacterAnimInstance.h"
#include "CombatAlliesAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum ECombatAlliesPlayerAnimationState
{
	CombatAlliesNone = 0x0000,
	CombatAlliesIsBlocking = 0x0001,
	CombatAlliesThrowingItem = 0x0002,
};

ENUM_CLASS_FLAGS(ECombatAlliesPlayerAnimationState)

UCLASS()
class REDEMPTION_API UCombatAlliesAnimInstance : public UCombatCharacterAnimInstance
{
	GENERATED_BODY()
public:
	UCombatAlliesAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void UpdateAnimProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "/Script/Redemption.ECombatAlliesPlayerAnimationState"), Category = "General Information")
		int32 CombatAlliesPlayerAnimationStateBitmaskCode {};
public:
	void ToggleCombatAlliesIsBlocking(bool Value);
	void ToggleCombatAlliesThrowingItem(bool Value);

	bool GetCombatAlliesIsBlocking() const;
};
