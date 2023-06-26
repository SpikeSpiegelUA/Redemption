// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "MyInputConfigData.generated.h"

/**
 * Store pointer to Input Action for native binding
 */
UCLASS()
class REDEMPTION_API UMyInputConfigData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputMove {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputLook {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputJump {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputAction {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputOpenPlayerMenu {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputScrollUp {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputScrollDown {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputScrollLeft {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputScrollRight {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputBack {};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UInputAction* InputPause {};
};