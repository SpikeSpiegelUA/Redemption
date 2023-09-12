// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\NonCombat\NonCombatEnemyNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\BitmapsActions.h"
#include "NonCombatEnemyNPCAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType, meta = (BitFlags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum ENonCombatEnemyNPCAnimationState
{
	NonCombatEnemyNPCNone = 0x0000,
	NonCombatEnemyNPCIsAttacking = 0x0001,
};

ENUM_CLASS_FLAGS(ENonCombatEnemyNPCAnimationState)

UCLASS()
class UNonCombatEnemyNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UNonCombatEnemyNPCAnimInstance();

	void SetNonCombatEnemyNPCIsAttacking(bool Value);

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Animation Properties")
		class ANonCombatEnemyNPC* NPCReference;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		float Speed{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "ENonCombatEnemyNPCAnimationState"), Category = "General Information")
		int32 NonCombatEnemyNPCAnimationStateBitmaskCode {};

	UFUNCTION(BlueprintCallable, Category = "Animations")
		virtual void UpdateAnimProperties();



};
