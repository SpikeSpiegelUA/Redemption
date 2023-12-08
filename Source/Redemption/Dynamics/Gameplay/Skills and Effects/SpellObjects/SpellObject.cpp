// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellObject.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "..\Dynamics\Gameplay\Skills and Effects\CreatedDebuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\PresetDebuffSpell.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\Dynamics\Gameplay\Skills and Effects\TurnStartDamageEffect.h"
#include "Kismet/GameplayStatics.h"
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
	if (IsValid(BattleManager) && IsValid(BattleManager->SelectedCombatNPC)) {
		//Interp is slowing down near the target, so we move target further
		FVector DirectionVector = BattleManager->SelectedCombatNPC->GetActorLocation() - this->GetActorLocation();
		DirectionVector.Normalize();
		FVector NewPosition = FMath::VInterpTo(this->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation() + DirectionVector * 400, DeltaTime, 1.5f);
		this->SetActorLocation(NewPosition);
	}
}

void ASpellObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (ACombatNPC* CombatNPC = Cast<ACombatNPC>(OtherActor); IsValid(CombatNPC)) {
			if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(Spell); IsValid(AssaultSpell)) {
				if (AssaultSpell->GetEffectsAndTheirChances().Num() > 0) {
					for (uint8 Index = 0; Index < AssaultSpell->GetEffectsAndTheirChances().Num(); Index++) {
						int8 Chance = FMath::RandRange(0, 100);
						ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(PlayerCharacter->GetBattleManager()->GetCombatFloatingInformationActorClass(), GetActorLocation(), GetActorRotation());
						FString TextForCombatFloatingInformationActor = FString();
						if (AssaultSpell->GetEffectsAndTheirChances()[Index].Chance >= Chance) {
							if (IsValid(Cast<ATurnStartDamageEffect>(AssaultSpell->GetEffectsAndTheirChances()[0].Effect->GetDefaultObject()))) {
								if (auto* TurnStartDamageEffect = NewObject<ATurnStartDamageEffect>(this, AssaultSpell->GetEffectsAndTheirChances()[0].Effect); IsValid(TurnStartDamageEffect))
									CombatNPC->Effects.Add(TurnStartDamageEffect);
							}
							else if (IsValid(Cast<AEffect>(AssaultSpell->GetEffectsAndTheirChances()[0].Effect->GetDefaultObject()))) {
								if (auto* Effect = NewObject<AEffect>(this, AssaultSpell->GetEffectsAndTheirChances()[0].Effect); IsValid(Effect)) {
									CombatNPC->Effects.Add(Effect);
									if (CombatNPC->CurrentHP > 0) {
										if (Effect->GetEffectType() == EEffectType::TURNSKIP)
											CombatNPC->GetMesh()->bPauseAnims = true;
										//else if (Effect->GetEffectType() == EEffectType::DIZZINESS)
										//	CombatNPC->GetMesh()->bPauseAnims = true;
									}
								}
							}
							TextForCombatFloatingInformationActor.Append("Extra effect applied!!!");
							CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
						}
						else {
							TextForCombatFloatingInformationActor.Append("Miss!");
							CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
						}
					}
				}
				CombatNPC->Execute_GetHit(CombatNPC, AssaultSpell->GetAttackValue(), ElementsActions::FindContainedElements(AssaultSpell->GetSpellElements()), false);
				OnOverlapBeginsActions(PlayerCharacter);
			}
			else if (APresetDebuffSpell* PresetDebuffSpell = Cast<APresetDebuffSpell>(Spell); IsValid(PresetDebuffSpell)) {
				TArray<AEffect*> EffectsArray;
				for (TSubclassOf<AEffect> EffectClass : PresetDebuffSpell->GetEffectsClasses())
					EffectsArray.Add(Cast<AEffect>(EffectClass->GetDefaultObject()));
				CombatNPC->Execute_GetHitWithBuffOrDebuff(CombatNPC, EffectsArray, ElementsActions::FindContainedElements(PresetDebuffSpell->GetSpellElements()));
				OnOverlapBeginsActions(PlayerCharacter);
			}
			else if (ACreatedDebuffSpell* CreatedDebuffSpell = Cast<ACreatedDebuffSpell>(Spell); IsValid(CreatedDebuffSpell)) {
				CombatNPC->Execute_GetHitWithBuffOrDebuff(CombatNPC, CreatedDebuffSpell->GetEffects(), ElementsActions::FindContainedElements(CreatedDebuffSpell->GetSpellElements()));
				OnOverlapBeginsActions(PlayerCharacter);
			}
		}
	}
}

void ASpellObject::OnOverlapBeginsActions(const class APlayerCharacter* const PlayerCharacter)
{
	PlayerCharacter->GetSpellBattleMenuWidget()->SetCreatedSpell(nullptr);
	PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell = false;
	PlayerCharacter->GetBattleManager()->SetTimerForPlayerTurnController();
	this->Destroy();
}

void ASpellObject::SetSpell(const class ASpell* const NewSpell)
{
	Spell = const_cast<ASpell*>(NewSpell);
}
