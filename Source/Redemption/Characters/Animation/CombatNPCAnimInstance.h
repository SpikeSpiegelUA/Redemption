// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\BitmapsActions.h"
#include "CombatNPCAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum ECombatEnemyNPCAnimationState
{
	CombatEnemyNPCNone = 0x0000,
	CombatEnemyNPCIsAttacking = 0x0001,
	CombatEnemyNPCIsDead = 0x0002,
	CombatEnemyNPCGotHit = 0x0004,
	CombatEnemyNPCIsAttackingWithMagic = 0x0008,
};

ENUM_CLASS_FLAGS(ECombatEnemyNPCAnimationState)

UCLASS()
class REDEMPTION_API UCombatNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UCombatNPCAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UPROPERTY(BlueprintReadOnly, Category = "Animation Properties")
		class ACombatEnemyNPC* NPCReference;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		float Speed{};
	UFUNCTION(BlueprintCallable, Category = "Animations")
		virtual void UpdateAnimProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "ECombatEnemyNPCAnimationState"), Category = "General Information")
		int32 CombatEnemyNPCAnimationStateBitmaskCode {};
public:
	void SetCombatEnemyNPCIsAttacking(bool Value);
	void SetCombatEnemyNPCIsDead(bool Value);
	void SetCombatEnemyNPCGotHit(bool Value);

	bool GetCombatEnemyNPCIsDead() const;
	bool GetCombatEnemyNPCGotHit() const;
};
