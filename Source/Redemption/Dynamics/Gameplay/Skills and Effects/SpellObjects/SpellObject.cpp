// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellObject.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\BattleManager.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\CreatedDebuffSpell.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Skills and Effects\PresetDebuffSpell.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpellObject::ASpellObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetBoxExtent(FVector(0.5f, 0.1f, 0.1f));
	BoxComponent->SetCollisionProfileName("Trigger");
	SetRootComponent(BoxComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASpellObject::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ASpellObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpellObject::Tick(float DeltaTime)
{
	//Move this object to the selected enemy
	Super::Tick(DeltaTime);
	ABattleManager* BattleManager{};
	TArray<AActor*> BattleManagerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABattleManager::StaticClass(), BattleManagerActors);
	if (BattleManagerActors.Num() > 0)
		BattleManager = Cast<ABattleManager>(BattleManagerActors[0]);
	if (IsValid(BattleManager) && IsValid(BattleManager->SelectedEnemy)) {
		//Interp is slowing down near the target, so we move target further
		FVector DirectionVector = BattleManager->SelectedEnemy->GetActorLocation() - this->GetActorLocation();
		DirectionVector.Normalize();
		FVector NewPosition = FMath::VInterpTo(this->GetActorLocation(), BattleManager->SelectedEnemy->GetActorLocation() + DirectionVector * 400, DeltaTime, 0.6f);
		this->SetActorLocation(NewPosition);
	}
}

void ASpellObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(OtherActor); IsValid(CombatEnemyNPC) && IsValid(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell())) {
			if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(Spell); IsValid(AssaultSpell)) {
				PlayerCharacter->GetBattleManager()->SelectedEnemy->Execute_GetHit(PlayerCharacter->GetBattleManager()->SelectedEnemy, AssaultSpell->GetAttackValue(), AssaultSpell->GetElementsAndTheirPercentagesStructs());
				OnOverlapBeginsActions(PlayerCharacter);
			}
			else if (APresetDebuffSpell* PresetDebuffSpell = Cast<APresetDebuffSpell>(Spell); IsValid(PresetDebuffSpell)) {
				TArray<AEffect*> EffectsArray;
				for (TSubclassOf<AEffect> EffectClass : PresetDebuffSpell->GetEffectsClasses())
					EffectsArray.Add(Cast<AEffect>(EffectClass->GetDefaultObject()));
				PlayerCharacter->GetBattleManager()->SelectedEnemy->Execute_GetHitWithBuffOrDebuff(PlayerCharacter->GetBattleManager()->SelectedEnemy, EffectsArray);
				OnOverlapBeginsActions(PlayerCharacter);
			}
			else if (ACreatedDebuffSpell* CreatedDebuffSpell = Cast<ACreatedDebuffSpell>(Spell); IsValid(CreatedDebuffSpell)) {
				PlayerCharacter->GetBattleManager()->SelectedEnemy->Execute_GetHitWithBuffOrDebuff(PlayerCharacter->GetBattleManager()->SelectedEnemy, CreatedDebuffSpell->GetEffects());
				OnOverlapBeginsActions(PlayerCharacter);
			}
		}
}

void ASpellObject::OnOverlapBeginsActions(const class APlayerCharacter* const& PlayerCharacter)
{
	PlayerCharacter->GetSkillBattleMenuWidget()->SetCreatedSpell(nullptr);
	PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell = false;
	PlayerCharacter->GetBattleManager()->SetTimerForPlayerTurnController();
	this->Destroy();
}

void ASpellObject::SetSpell(class ASpell* NewSpell)
{
	Spell = NewSpell;
}
