// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "../Dynamics/Gameplay/Managers/BattleManager.h"
#include "ItemObject.generated.h"

UCLASS()
class REDEMPTION_API AItemObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetItem(class AGameItem* NewItem);
	void SetTargetBattleSide(const EBattleSide NewTargetBattleSide);
private:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		UBoxComponent* BoxComponent {};

	class AGameItem* Item{};
	EBattleSide TargetBattleSide{};
	//On overlap with the player or an enemy we need to destroy this object and set timer for turn change
	void OnOverlapBeginsActions(const class APlayerCharacter* const PlayerCharacter);
};
