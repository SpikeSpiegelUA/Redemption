// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include <Kismet/GameplayStatics.h>
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\AssaultItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\World\Items\DebuffItem.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
AItemObject::AItemObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(0.5f, 0.1f, 0.1f));
	BoxComponent->SetCollisionProfileName("Trigger");
	SetRootComponent(BoxComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemObject::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AItemObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ABattleManager* BattleManager{};
	TArray<AActor*> BattleManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleManager::StaticClass(), BattleManagerActors);
	if (BattleManagerActors.Num() > 0)
		BattleManager = Cast<ABattleManager>(BattleManagerActors[0]);
	if (IsValid(BattleManager) && IsValid(BattleManager->SelectedCombatNPC)) {
		//Interp is slowing down near the target, so we move target further
		FVector DirectionVector = BattleManager->SelectedCombatNPC->GetActorLocation() - this->GetActorLocation();
		DirectionVector.Normalize();
		FVector NewPosition = FMath::VInterpTo(this->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation() + DirectionVector * 400, DeltaTime, 0.6f);
		this->SetActorLocation(NewPosition);
	}
}

void AItemObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(OtherActor); IsValid(CombatEnemyNPC)) {
			if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(Item); IsValid(AssaultItem)) {
				PlayerCharacter->GetBattleManager()->SelectedCombatNPC->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedCombatNPC,
					AssaultItem->GetAttackValue(), AssaultItem->GetElementsAndTheirPercentagesStructs(), false);
				OnOverlapBeginsActions(PlayerCharacter);
			}
			else if (ADebuffItem* DebuffItem = Cast<ADebuffItem>(Item); IsValid(DebuffItem)) {
				TArray<AEffect*> EffectsArray;
				for (TSubclassOf<AEffect> EffectClass : DebuffItem->GetEffectsClasses())
					EffectsArray.Add(Cast<AEffect>(EffectClass->GetDefaultObject()));
				PlayerCharacter->GetBattleManager()->SelectedCombatNPC->Execute_GetHitWithBuffOrDebuff(PlayerCharacter->GetBattleManager()->SelectedCombatNPC, EffectsArray);
				OnOverlapBeginsActions(PlayerCharacter);
			}
		}
}

void AItemObject::OnOverlapBeginsActions(const APlayerCharacter* const& PlayerCharacter)
{
	PlayerCharacter->GetInventoryMenuWidget()->SetPickedItem(nullptr);
	PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithItem = false;
	PlayerCharacter->GetBattleManager()->SetTimerForPlayerTurnController();
	this->Destroy();
}

void AItemObject::SetItem(AGameItem* NewItem)
{
	Item = NewItem;
}

