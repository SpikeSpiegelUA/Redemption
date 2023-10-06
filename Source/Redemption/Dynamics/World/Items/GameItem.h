// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Miscellaneous\ElementAndItsPercentage.h"
#include "GameItem.generated.h"

UCLASS()
class AGameItem : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGameItem();

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		FName Name = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		int Cost{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information", meta = (AllowPrivateAccess = true))
		FText Description{};



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName GetItemName() const;
	int GetCost() const;
	FText GetDescription() const;
};
