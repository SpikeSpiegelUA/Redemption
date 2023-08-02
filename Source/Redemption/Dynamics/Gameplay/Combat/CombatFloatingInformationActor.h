// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\CombatFloatingInformation.h"
#include "Components/WidgetComponent.h"
#include "CombatFloatingInformationActor.generated.h"

UCLASS()
class ACombatFloatingInformationActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACombatFloatingInformationActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCombatFloatingInformationText(FText& NewCombatFloatingInformationText);

private:
	UPROPERTY(EditAnywhere, Category = "UI", meta = (AllowPrivateAccess = true))
		TSubclassOf<class UCombatFloatingInformation> CombatFloatingInformationClass{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = true))
		UCombatFloatingInformation* CombatFloatingInformationWidget {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = true))
		UWidgetComponent* CombatFloatingInformationComponentWidget;

	FText CombatFloatingInformationText{};
};
