// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Redemption/Dynamics/Scripting/ScriptingActor.h"
#include "Redemption/Dynamics/Miscellaneous/QuestAndItsStage.h"
#include "ChangeQuestStageTrigger.generated.h"

/**
 * 
 */
UCLASS()
class REDEMPTION_API AChangeQuestStageTrigger : public AScriptingActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AChangeQuestStageTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Change quest stage trigger")
	FQuestAndItsStage QuestAndItsStageToTrigger{};
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Change quest stage trigger")
	int NewStage{};

	UPROPERTY(EditAnywhere, BLueprintReadOnly, Category = "Change quest stage trigger")
	class UBoxComponent* BoxComponent{};
};
