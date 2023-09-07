// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Delegates/Delegate.h"
#include "ResponseEntry.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseClicked, const FText&, ResponseText);

UCLASS()
class REDEMPTION_API UResponseEntry : public UUserWidget
{
	GENERATED_BODY()
private:
	FText ResponseText = FText::FromString("");

	UFUNCTION()
		void ResponseButtonOnClicked();
	UFUNCTION()
		void ResponseButtonOnHovered();
	UFUNCTION()
		void ResponseButtonOnUnhovered();
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* ResponseTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* ResponseButton;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
public:

	UTextBlock* GetResponseTextBlock() const;
	UFUNCTION()
	FText GetResponseText() const;

	UPROPERTY(BlueprintAssignable, Category = "Test")
		FResponseClicked OnResponseClicked;

	void SetResponseText(const FText& NewText);
};
