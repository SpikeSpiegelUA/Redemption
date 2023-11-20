#include "GameItemWithElements.h"

// Sets default values
AGameItemWithElements::AGameItemWithElements()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameItemWithElements::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameItemWithElements::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<FElementAndItsPercentageStruct> AGameItemWithElements::GetElementsAndTheirPercentagesStructs() const
{
	return ElementsAndTheirPercentagesStructs;
}