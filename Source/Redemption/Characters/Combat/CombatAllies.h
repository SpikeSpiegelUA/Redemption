// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatNPC.h"
#include "CombatAllies.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API ACombatAllies : public ACombatNPC
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACombatAllies();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		class UFloatingManaBarWidget* FloatingManaBarWidget {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
		UWidgetComponent* FloatingManaBarComponentWidget;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Variables for movement in battle scene
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle")
		bool IsMovingToAttackEnemy = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Battle")
		bool IsMovingToStartPosition = false;

	void StartMovingToEnemy();
	void StartMovingToStartLocation();

	//When aiming with a range weapon, center mark is flying around the screen with a certain speed. Set that speed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
		float CenterMarkMovingSpeed{};

	UFloatingManaBarWidget* GetFloatingManaBarWidget() const;
};
