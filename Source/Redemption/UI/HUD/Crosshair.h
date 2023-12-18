// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Crosshair.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UCanvasPanel* MainCanvasPanel;
public:
	UCanvasPanel* GetMainCanvasPanel() const;
};
