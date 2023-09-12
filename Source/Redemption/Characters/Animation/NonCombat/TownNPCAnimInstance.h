// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\NonCombat\TownNPC.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\BitmapsActions.h"
#include "TownNPCAnimInstance.generated.h"

/**
 * 
 */
enum ETownNPCAnimationState
{
	TownNPCNone = 0x0000,
	TownNPCIsAlive = 0x0001,
};

ENUM_CLASS_FLAGS(ETownNPCAnimationState)

UCLASS()
class UTownNPCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UTownNPCAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Animation Properties")
		class ATownNPC* NPCReference;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation Properties")
		float Speed{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "ETownNPCAnimationState"), Category = "General Information")
		int32 TownNPCAnimationStateBitmaskCode {};

	UFUNCTION(BlueprintCallable, Category = "Animations")
		virtual void UpdateAnimProperties();

};
