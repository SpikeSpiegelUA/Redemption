// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\NPCAIController.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\SmartObjects\SmartObject.h"
#include "CharacterInTheWorld.generated.h"

UCLASS()
class ACharacterInTheWorld : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Character name, that will be displayed to the player, set by designer
	UPROPERTY(EditAnywhere, Category = "General Information")
		FName CharacterName = "Unassigned";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		class UBehaviorTree* TreeAsset{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		class ASmartObject* SmartObject{};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sets default values for this character's properties
	ACharacterInTheWorld();

	FName GetCharacterName() const;

	UBehaviorTree* GetTreeAsset() const;

	ASmartObject* GetSmartObject() const;
	
	void SetSmartObject(ASmartObject* Object);
};
