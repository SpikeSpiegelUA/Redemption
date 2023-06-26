// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\NonCombatEnemyNPC.h"
#include "NonCombatEnemyNPCAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNonCombatEnemyNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UNonCombatEnemyNPCAnimInstance();

	void SetIsAttacking(bool Value);

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Animation Properties")
		class ANonCombatEnemyNPC* NPCReference;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		float Speed{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		bool IsAttacking{};

	UFUNCTION(BlueprintCallable, Category = "Animations")
		virtual void UpdateAnimProperties();

};
