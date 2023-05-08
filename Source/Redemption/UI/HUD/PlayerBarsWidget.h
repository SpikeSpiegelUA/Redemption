// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "PlayerBarsWidget.generated.h"
/**
 * 
 */
UCLASS()
class REDEMPTION_API UPlayerBarsWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	APlayerCharacter* PlayerCharacter;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* PlayerHealthBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* PlayerManaBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* PlayerBarsCanvasPanel;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
		float GetHealthPercentage();
	UFUNCTION()
		float GetManaPercentage();

public:
	UProgressBar* GetPlayerHealthBar();
    UProgressBar* GetPlayerManaBar();
	UCanvasPanel* GetPlayerBarsCanvasPanel();
};
