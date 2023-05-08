// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public: 
	UPlayerCharacterAnimInstance();
	bool GetIsInAir();
	
	void SetIsAttacking(bool Value);
	void SetSpeed(float Value);
	void SetIsDead(bool Value);

	//In battle mode to play "Run" animation, Speed must be assigned manually to 300 and not to actual player speed. So we block NativeUpdateAnimation of this variable with this bool
	bool SetSpeedToActualSpeed = true;
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		class APlayerCharacter* PlayerReference;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		bool IsAttacking;

	UFUNCTION(BlueprintCallable, Category = "Animations")
		virtual void UpdateAnimProperties();
	
};
