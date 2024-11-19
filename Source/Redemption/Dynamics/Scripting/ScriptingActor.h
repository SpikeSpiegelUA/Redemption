// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "..\Source\Redemption\SaveSystem\Interfaces\SavableObjectInterface.h"
#include "ScriptingActor.generated.h"

UCLASS()
class REDEMPTION_API AScriptingActor : public AActor, public ISavableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScriptingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Whether to destroy this object after the trigget.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Scripting actor")
	bool DeactivateAfterTrigger{};
	//Whether to deactivated this object after the trigget.
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Scripting actor")
	bool DestroyAfterTrigger;

	void LoadObjectFromGameInstance_Implementation(const URedemptionGameInstance* const GameInstance) override;
	//Call this function in children's OnCollision functions or anywhere else they are triggered.
	void DeactivationAndDestroyLogic();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scripting actor", SaveGame)
	bool Activated{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scripting actor", SaveGame)
	bool Triggered = false;
};
