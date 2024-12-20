// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "..\Dynamics\Gameplay\Managers\BattleManager.h"
#include "Kismet/GameplayStatics.h"
#include "..\Dynamics\World\Items\UseItems\AssaultItem.h"
#include "..\Dynamics\World\Items\UseItems\DebuffItem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Redemption\Dynamics\Gameplay\Skills and Effects\Effects\TurnStartDamageEffect.h"
#include "Redemption/Miscellaneous/ElementsActions.h"
#include "..\Miscellaneous\InventoryActions.h"
#include "NiagaraFunctionLibrary.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
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
		FVector NewPosition = FMath::VInterpTo(this->GetActorLocation(), BattleManager->SelectedCombatNPC->GetActorLocation() + DirectionVector * 400, DeltaTime, 1.5f);
		this->SetActorLocation(NewPosition);
	}
}

void AItemObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
		if (ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(OtherActor); IsValid(CombatEnemyNPC) && IsValid(Item)) {
			//Cast for each type of spell.
			//Need to create an Array of actors that were hit.
			for (ACombatNPC* CombatTarget : InventoryActions::GetTargets(RedemptionGameModeBase->GetBattleManager(), TargetBattleSide, Item->GetItemRange())) {
				if (CombatTarget->CurrentHP > 0) {
					if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(Item); IsValid(AssaultItem)) {
						//Logic for special effects of the spell(frozen, burn...).
						if (CombatTarget->Execute_GetHit(CombatTarget, AssaultItem->GetAttackValue(), NPCOwner, AssaultItem->GetElementsAndTheirPercentagesStructs(),
							EPhysicalType::NONE, 0, 0, false)) {
							if (AssaultItem->GetEffectsAndTheirChances().Num() > 0) {
								for (uint8 Index = 0; Index < AssaultItem->GetEffectsAndTheirChances().Num(); Index++) {
									int8 Chance = FMath::RandRange(0, 100);
									ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(RedemptionGameModeBase->GetBattleManager()->GetCombatFloatingInformationActorClass(), CombatTarget->GetActorLocation(), GetActorRotation());
									FString TextForCombatFloatingInformationActor = FString();
									if (AssaultItem->GetEffectsAndTheirChances()[Index].Chance >= Chance) {
										if (IsValid(Cast<ATurnStartDamageEffect>(AssaultItem->GetEffectsAndTheirChances()[Index].Effect->GetDefaultObject()))) {
											if (auto* TurnStartDamageEffect = NewObject<ATurnStartDamageEffect>(this, AssaultItem->GetEffectsAndTheirChances()[Index].Effect); IsValid(TurnStartDamageEffect)) {
												TurnStartDamageEffect->InitializeObject(NPCOwner);
												CombatTarget->Effects.Add(TurnStartDamageEffect);
												if (ElementsActions::FindSpellsMainElement(TurnStartDamageEffect->GetSpellElements()) == ESpellElements::FIRE) {
													if (IsValid(RedemptionGameModeBase->GetParticlesManager()) && IsValid(RedemptionGameModeBase->GetParticlesManager()->GetFlamesEmitter()))
														if (CombatTarget->CurrentHP > 0)
															CombatTarget->FlamesEmitterComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(RedemptionGameModeBase->GetParticlesManager()->GetFlamesEmitter(),
																CombatTarget->GetRootComponent(), NAME_None, FVector(0, 0, 45), FRotator(0, 0, 0), EAttachLocation::SnapToTarget, false, true, ENCPoolMethod::None, true);
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
										else if (IsValid(Cast<AEffect>(AssaultItem->GetEffectsAndTheirChances()[Index].Effect->GetDefaultObject()))) {
											if (auto* Effect = NewObject<AEffect>(this, AssaultItem->GetEffectsAndTheirChances()[Index].Effect); IsValid(Effect)) {
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
														if (IsValid(RedemptionGameModeBase->GetParticlesManager()) && IsValid(RedemptionGameModeBase->GetParticlesManager()->GetDizzyEmitter()))
															if (CombatTarget->CurrentHP > 0)
																CombatTarget->DizzyEmitterComponent = UGameplayStatics::SpawnEmitterAttached(RedemptionGameModeBase->GetParticlesManager()->GetDizzyEmitter(), CombatTarget->GetRootComponent(), NAME_None,
																	FVector(0, 0, 110), FRotator(0, 0, 0), FVector(1, 1, 1), EAttachLocation::SnapToTarget, false, EPSCPoolMethod::AutoRelease);
													}
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
						}
						OnOverlapBeginsActions();
					}
					else if (ADebuffItem* DebuffItem = Cast<ADebuffItem>(Item); IsValid(DebuffItem)) {
						TArray<AEffect*> EffectsArray;
						for (TSubclassOf<AEffect> EffectClass : DebuffItem->GetEffectsClasses()) {
							AEffect* NewEffect = NewObject<AEffect>(this, EffectClass);
							EffectsArray.Add(NewEffect);
						}
						CombatTarget->Execute_GetHitWithBuffOrDebuff(CombatTarget, EffectsArray, DebuffItem->GetElementsAndTheirPercentagesStructs(), 0, 0, NPCOwner, ESpellType::DEBUFF);
						OnOverlapBeginsActions();
					}
				}
			}
		}
	}
}

void AItemObject::OnOverlapBeginsActions()
{
	if (const auto* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		RedemptionGameModeBase->GetBattleManager()->SetTimerForPlayerTurnController();
	if (const auto* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem)) {
		UIManagerWorldSubsystem->InventoryMenuWidget->SetPickedItem(nullptr);
		UIManagerWorldSubsystem->BattleMenuWidget->IsAttackingWithItem = false;
	}
		this->Destroy();
}

void AItemObject::SetItem(AGameItem* NewItem)
{
	Item = NewItem;
}

void AItemObject::SetNPCOwner(const ACombatNPC* const NewNPCOwner)
{
	NPCOwner = const_cast<ACombatNPC*>(NewNPCOwner);
}

void AItemObject::SetTargetBattleSide(const EBattleSide NewUserBattleSide)
{
	TargetBattleSide = NewUserBattleSide;
}

