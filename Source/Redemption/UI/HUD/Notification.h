// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Notification.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UNotification : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* NotificationTextBlock;

	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

public:

	void SetNotificationTextBlockText(FText const& NewText);
};
