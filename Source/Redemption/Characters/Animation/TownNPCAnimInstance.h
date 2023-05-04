// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Other\TownNPC.h"
#include "TownNPCAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UTownNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UTownNPCAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	class ATownNPC* NPCReference;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		bool IsAlive;
	UFUNCTION(BlueprintCallable, Category = "Animations")
		virtual void UpdateAnimProperties();

};
