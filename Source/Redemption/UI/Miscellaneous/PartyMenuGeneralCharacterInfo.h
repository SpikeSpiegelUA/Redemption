// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"
#include "PartyMenuGeneralCharacterInfo.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API UPartyMenuGeneralCharacterInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	class UButton* CharacterNameButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	class UImage* CharacterPortraitImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* CharacterNameTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* CharacterLevelTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* CharacterHPTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	class UTextBlock* CharacterManaTextBlock;

	UFUNCTION()
	void CharacterNameButtonOnClicked();
	UFUNCTION()
	void CharacterNameButtonOnHovered();
public:
	void SetCharacterPortraitImage(const UTexture* const ImageToSet);
	void SetCharacterNameTextBlockText(const FText& TextToSet);
	void SetCharacterLevelTextBlockText(const FText& TextToSet);
	void SetCharacterHPTextBlockText(const FText& TextToSet);
	void SetCharacterManaTextBlockText(const FText& TextToSet);
	void SetCharacterInfo(const ACombatAllyNPC* const AllyToSet);
	void SetCharacterInfo(const APlayerCharacter* const PlayerToSet);

	ACombatAllyNPC* Ally{};
};
