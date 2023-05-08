// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "ResponsesBox.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UResponsesBox : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UVerticalBox* ResponseVerticalBox;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
public:

	UVerticalBox* GetResponseVerticalBox();
};
