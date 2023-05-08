// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CombatNPCAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UCombatNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UCombatNPCAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	class ACombatEnemyNPC* NPCReference;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		bool IsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		bool IsDead;
	UFUNCTION(BlueprintCallable, Category = "Animations")
		virtual void UpdateAnimProperties();

public:
	void SetIsAttacking(bool Value);
	void SetIsDead(bool Value);

	bool GetIsDead();
};
