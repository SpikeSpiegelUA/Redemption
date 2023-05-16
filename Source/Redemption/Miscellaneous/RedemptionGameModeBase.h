// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\Menus\MainMenu.h"
#include "RedemptionGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ARedemptionGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UMainMenu> MainMenuClass;
	UPROPERTY()
		class UMainMenu* MainMenuWidget;

	ARedemptionGameModeBase();
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
};
