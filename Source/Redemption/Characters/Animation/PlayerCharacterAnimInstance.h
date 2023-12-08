// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "..\Miscellaneous\BitmapsActions.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum EPlayerCharacterAnimationState
{
	PlayerCharacterAnimationStateNone = 0x0000,
	PlayerIsAttacking = 0x0001,
	PlayerIsBlock = 0x0002,
	PlayerGotHit = 0x0004,
	PlayerIsThrowing = 0x0008,
	PlayerIsAttackingWithMagic = 0x0010,
	PlayerIsIdle = 0x0020,
	PlayerIsInAir = 0x0040,
	PlayerIsDead = 0x0080,
};

ENUM_CLASS_FLAGS(EPlayerCharacterAnimationState)

UCLASS()
class UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public: 
	UPlayerCharacterAnimInstance();
	bool GetPlayerIsInAir() const;
	bool GetPlayerIsBlock() const;
	bool GetPlayerIsThrowing() const;
	bool GetPlayerIsAttackingWithMagic() const;

	void SetPlayerIsAttacking(bool Value);
	void SetPlayerSpeed(float Value);
	void SetPlayerIsDead(bool Value);
	void SetPlayerIsBlock(bool Value);
	void SetPlayerGotHit(bool Value);
	void SetPlayerIsThrowing(bool Value);
	void SetPlayerIsAttackingWithMagic(bool Value);
	void SetPlayerIsAiming(bool Value);

	//In battle mode to play "Run" animation, Speed must be assigned manually to 300 and not to actual player speed. So we block NativeUpdateAnimation of this variable with this bool
	bool SetSpeedToActualSpeed = true;
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "/Script/Redemption.EPlayerCharacterAnimationState"), Category = "General Information")
		int32 PlayerCharacterAnimationStateBitmaskCode {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		class APlayerCharacter* PlayerReference{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		float Speed{};

	UFUNCTION(BlueprintCallable, Category = "Animations")
		virtual void UpdateAnimProperties();
	
};
