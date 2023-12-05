#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameItem.h"
#include "GameItemWithElements.generated.h"

UCLASS()
class REDEMPTION_API AGameItemWithElements : public AGameItem
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Information", meta = (AllowPrivateAccess = true))
		TArray<FElementAndItsPercentageStruct> ElementsAndTheirPercentagesStructs{};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Sets default values for this actor's properties
	AGameItemWithElements();
	TArray<FElementAndItsPercentageStruct> GetElementsAndTheirPercentagesStructs() const;
};
