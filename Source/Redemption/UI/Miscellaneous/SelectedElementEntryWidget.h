// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\Effect.h"
#include "SelectedElementEntryWidget.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API USelectedElementEntryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UImage* MainImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = true))
		class UButton* MainButton;

	ESpellElements Element{};

	UFUNCTION()
		void MainButtonOnClicked();

public:
	void SetElement(ESpellElements NewElement);

	ESpellElements GetElement() const;
	UImage* GetMainImage() const;
};
