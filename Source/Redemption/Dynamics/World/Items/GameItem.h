// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameItem.generated.h"

UENUM(BlueprintType)
enum class ItemType :uint8
{
	ASSAULT UMETA(DisplayName = "Assault"),
	SUPPORT UMETA(DisplayName = "Support"),
	MISCELLANEOUS UMETA(DisplayName = "Miscellaneous"),
	EQUIPMENT UMETA(DisplayName = "Equipment"),
};

UCLASS()
class AGameItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General Information")
	FName Name = "";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General information");
	ItemType Type;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName GetItemName() const;
	ItemType GetType();

};
