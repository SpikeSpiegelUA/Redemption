// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\CombatFloatingInformation.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
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

	void SetCombatFloatingInformationText(const FText& NewCombatFloatingInformationText);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = true))
		UCombatFloatingInformation* CombatFloatingInformationWidget {};

	UFUNCTION()
		void DestroyThisActor();
	FTimerHandle ActorDestroyTimerHandle;
};
