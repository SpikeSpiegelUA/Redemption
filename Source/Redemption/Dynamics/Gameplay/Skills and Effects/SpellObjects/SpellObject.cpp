// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellObject.h"
#include "..\Dynamics\Gameplay\Skills and Effects\CreatedDebuffSpell.h"
#include "..\Dynamics\Gameplay\Skills and Effects\PresetDebuffSpell.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\Dynamics\Gameplay\Skills and Effects\TurnStartDamageEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "D:/UE_5.1/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "D:/UE_5.1/Engine/Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"

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
	if (IsValid(Target)) {
		//Interp is slowing down near the target, so we move target further
		FVector DirectionVector = Target->GetActorLocation() - this->GetActorLocation();
		DirectionVector.Normalize();
		FVector NewPosition = FMath::VInterpTo(this->GetActorLocation(), Target->GetActorLocation() + DirectionVector * 400, DeltaTime, 1.5f);
		this->SetActorLocation(NewPosition);
	}
}

void ASpellObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (ACombatNPC* CombatNPC = Cast<ACombatNPC>(OtherActor); IsValid(CombatNPC) && IsValid(Spell)) {
			//Cast for each type of spell.
			//Need to create an Array of actors that were hit.
			TArray<ACombatNPC*> TargetActors{};
			if (TargetBattleSide == EBattleSide::ENEMIES)
				TargetActors = SkillsSpellsAndEffectsActions::GetTargetsForAllies(PlayerCharacter->GetBattleManager(), Spell->GetSpellRange(), Spell->GetTypeOfSpell());
			else if(TargetBattleSide == EBattleSide::ALLIES)
				TargetActors = SkillsSpellsAndEffectsActions::GetTargetsForEnemies(PlayerCharacter->GetBattleManager(), Target, Spell->GetSpellRange(), Spell->GetTypeOfSpell());
			for (ACombatNPC* CombatTarget : TargetActors) {
				if (CombatTarget->CurrentHP > 0) {
					if (AAssaultSpell* AssaultSpell = Cast<AAssaultSpell>(Spell); IsValid(AssaultSpell)) {
						//Logic for special effects of the spell(frozen, burn...).
						if (CombatTarget->Execute_GetHit(CombatTarget, AssaultSpell->GetAttackValue(), ElementsActions::FindContainedElements(AssaultSpell->GetSpellElements()), false)) {
							if (AssaultSpell->GetEffectsAndTheirChances().Num() > 0) {
								for (uint8 Index = 0; Index < AssaultSpell->GetEffectsAndTheirChances().Num(); Index++) {
									int8 Chance = FMath::RandRange(0, 100);
									ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(PlayerCharacter->GetBattleManager()->GetCombatFloatingInformationActorClass(), GetActorLocation(), GetActorRotation());
									FString TextForCombatFloatingInformationActor = FString();
									if (AssaultSpell->GetEffectsAndTheirChances()[Index].Chance >= Chance) {
										if (IsValid(Cast<ATurnStartDamageEffect>(AssaultSpell->GetEffectsAndTheirChances()[Index].Effect->GetDefaultObject()))) {
											if (auto* TurnStartDamageEffect = NewObject<ATurnStartDamageEffect>(this, AssaultSpell->GetEffectsAndTheirChances()[Index].Effect); IsValid(TurnStartDamageEffect)) {
												//Check if Effects array already has this TurnStartDamageEffect. If yes, then add damage and update duration of an existent, if no, then add new effect.
												bool EffectAlreadyIsInArray = false;
												for (AEffect* Effect : CombatTarget->Effects)
													if (Effect->GetEffectName().EqualTo(TurnStartDamageEffect->GetEffectName())) {
														EffectAlreadyIsInArray = true;
														Effect->CurrentDuration = 0;
														Effect->SetEffectStat(Effect->GetEffectStat() + TurnStartDamageEffect->GetEffectStat());
														break;
													}
												if (!EffectAlreadyIsInArray) {
													CombatTarget->Effects.Add(TurnStartDamageEffect);
													if (ElementsActions::FindSpellsMainElement(TurnStartDamageEffect->GetSpellElements()) == ESpellElements::FIRE) {
														if (IsValid(PlayerCharacter->GetParticlesManager()) && IsValid(PlayerCharacter->GetParticlesManager()->GetFlamesEmitter()))
															CombatTarget->FlamesEmitterComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(PlayerCharacter->GetParticlesManager()->GetFlamesEmitter(),
																CombatTarget->GetRootComponent(), NAME_None, FVector(0, 0, 39), FRotator(0, 0, 0), EAttachLocation::SnapToTarget, false, true, ENCPoolMethod::None, true);
														//If Fire elements, then remove frozen effect.
														for (AEffect* Effect : CombatTarget->Effects)
															if (Effect->GetEffectType() == EEffectType::TURNSKIP) {
																CombatTarget->Effects.Remove(Effect);
																CombatTarget->GetMesh()->bPauseAnims = false;
																break;
															}
													}
												}
											}
										}
										else if (IsValid(Cast<AEffect>(AssaultSpell->GetEffectsAndTheirChances()[Index].Effect->GetDefaultObject()))) {
											if (auto* Effect = NewObject<AEffect>(this, AssaultSpell->GetEffectsAndTheirChances()[Index].Effect); IsValid(Effect)) {
												//Check if Effects array already has this effect. If yes, set CurrentDuration to 0 of an existent, if no, then add new effect.
												bool EffectAlreadyIsInArray = false;
												for (AEffect* ExistingEffect : CombatTarget->Effects)
													if (ExistingEffect->GetEffectName().EqualTo(Effect->GetEffectName())) {
														EffectAlreadyIsInArray = true;
														ExistingEffect->CurrentDuration = 0;
														break;
													}
												if (!EffectAlreadyIsInArray) {
													CombatTarget->Effects.Add(Effect);
													if (CombatTarget->CurrentHP > 0) {
														if (Effect->GetEffectType() == EEffectType::TURNSKIP) {
															CombatTarget->GetMesh()->bPauseAnims = true;
															//Remove burn effect
															for (AEffect* ExtraEffect : CombatTarget->Effects)
																if (ExtraEffect->GetEffectType() == EEffectType::TURNSTARTDAMAGE)
																	if (ATurnStartDamageEffect* TurnStartDamageEffect = Cast<ATurnStartDamageEffect>(ExtraEffect); IsValid(TurnStartDamageEffect))
																		if (ElementsActions::FindSpellsMainElement(TurnStartDamageEffect->GetSpellElements()) == ESpellElements::FIRE) {
																			CombatTarget->Effects.Remove(ExtraEffect);
																			if (IsValid(CombatTarget->FlamesEmitterComponent))
																				CombatTarget->FlamesEmitterComponent->DestroyComponent();
																			break;
																		}
														}
														else if (Effect->GetEffectType() == EEffectType::DIZZINESS) {
															if (IsValid(PlayerCharacter->GetParticlesManager()) && IsValid(PlayerCharacter->GetParticlesManager()->GetDizzyEmitter()))
																CombatTarget->DizzyEmitterComponent = UGameplayStatics::SpawnEmitterAttached(PlayerCharacter->GetParticlesManager()->GetDizzyEmitter(), CombatTarget->GetRootComponent(), NAME_None,
																	FVector(0, 0, 110), FRotator(0, 0, 0), FVector(1, 1, 1), EAttachLocation::SnapToTarget, false, EPSCPoolMethod::AutoRelease);
														}
													}
												}
											}
										}
										TextForCombatFloatingInformationActor.Append("Extra effect applied!!!");
										CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
									}
									else {
										TextForCombatFloatingInformationActor.Append("Extra effect missed!");
										CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
									}
								}
							}
						}
						OnOverlapBeginsActions(PlayerCharacter);
					}
					else if (APresetDebuffSpell* PresetDebuffSpell = Cast<APresetDebuffSpell>(Spell); IsValid(PresetDebuffSpell)) {
						TArray<AEffect*> EffectsArray;
						for (TSubclassOf<AEffect> EffectClass : PresetDebuffSpell->GetEffectsClasses()) {
							AEffect* NewEffect = NewObject<AEffect>(this, EffectClass);
							EffectsArray.Add(NewEffect);
						}
						CombatTarget->Execute_GetHitWithBuffOrDebuff(CombatTarget, EffectsArray, ElementsActions::FindContainedElements(PresetDebuffSpell->GetSpellElements()), PresetDebuffSpell->GetTypeOfSpell());
						OnOverlapBeginsActions(PlayerCharacter);
					}
					else if (ACreatedDebuffSpell* CreatedDebuffSpell = Cast<ACreatedDebuffSpell>(Spell); IsValid(CreatedDebuffSpell)) {
						TArray<AEffect*> EffectsArray;
						for (AEffect* Effect : CreatedDebuffSpell->GetEffects()) {
							AEffect* NewEffect = NewObject<AEffect>(this);
							NewEffect->CopyEffect(Effect);
							EffectsArray.Add(NewEffect);
						}
						CombatTarget->Execute_GetHitWithBuffOrDebuff(CombatTarget, EffectsArray, ElementsActions::FindContainedElements(CreatedDebuffSpell->GetSpellElements()), CreatedDebuffSpell->GetTypeOfSpell());
						OnOverlapBeginsActions(PlayerCharacter);
					}
				}
			}
		}
	}
}

void ASpellObject::OnOverlapBeginsActions(const class APlayerCharacter* const PlayerCharacter)
{
	if (TargetBattleSide == EBattleSide::ENEMIES){
		PlayerCharacter->GetSpellBattleMenuWidget()->SetCreatedSpell(nullptr);
		PlayerCharacter->GetBattleMenuWidget()->IsAttackingWithSpell = false;
		PlayerCharacter->GetBattleManager()->SetTimerForPlayerTurnController();
	}
	this->Destroy();
}

void ASpellObject::SetSpell(const class ASpell* const NewSpell)
{
	Spell = const_cast<ASpell*>(NewSpell);
}

void ASpellObject::SetTargetBattleSide(const EBattleSide NewTargetBattleSide)
{
	TargetBattleSide = NewTargetBattleSide;
}

void ASpellObject::SetTarget(ACombatNPC* const NewTarget)
{
	Target = NewTarget;
}
