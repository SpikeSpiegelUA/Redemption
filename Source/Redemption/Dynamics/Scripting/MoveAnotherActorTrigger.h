// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "..\Source\Redemption\Dynamics\Logic\SmartObjects\SmartObject.h"
#include "..\Source\Redemption\Dynamics\Miscellaneous\QuestAndItsStage.h"
#include "ScriptingActor.h"
#include "MoveAnotherActorTrigger.generated.h"

//This one triggers when the Player enters the trigger area.
UCLASS()
class REDEMPTION_API AMoveAnotherActorTrigger : public AScriptingActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveAnotherActorTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move another actor trigger")
	AActor* ActorToMove{};

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move another actor trigger")
	FVector NewLocation{};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move another actor trigger")
	FRotator NewRotation{};
	//If we need to change behavior after the trigger, set it here. If this variable is null, then behavior will be set to nullptr.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move another actor trigger")
	ASmartObject* NewSmartObject{};
	//We need to select quests and stages, at which this trigger is activated.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Move another actor trigger")
	TArray<FQuestAndItsStage> QuestsAndTheirStagesToTrigger{};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Scripting actor")
	bool AdvanceQuestAfterTrigger;

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = "Move another actor trigger")
	UBoxComponent* BoxComponent{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
