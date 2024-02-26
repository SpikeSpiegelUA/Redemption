// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "..\Dynamics\World\Items\GameItem.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "SpellObject.generated.h"

//Spell's "object" class. For example, player is throwind red rectangle(well, this is a spell's object) when using a fire arrow.
UCLASS()
class ASpellObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent{};

	class ASpell* Spell{};
	EBattleSide TargetBattleSide{};
	ACombatNPC* Target{};
	ACombatNPC* NPCOwner{};

	//On overlap with the player or an enemy we need to destroy this object and set timer for turn change
	void OnOverlapBeginsActions();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSpell(const class ASpell* const NewSpell);
	void SetTargetBattleSide(const EBattleSide NewTargetBattleSide);
	void SetTarget(ACombatNPC* const NewTarget);
	void SetNPCOwner(const ACombatNPC* const NewNPCOwner);
};
