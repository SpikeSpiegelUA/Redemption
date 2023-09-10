// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAllies.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\Combat\CombatAlliesAIController.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\Combat\CombatCharacterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\UI\HUD\FloatingManaBarWidget.h"

ACombatAllies::ACombatAllies()
{
	//Create ComponentWidget for FloatingManaBar
	FloatingManaBarComponentWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Mana Bar"));
	FloatingManaBarComponentWidget->SetWidgetSpace(EWidgetSpace::Screen);
	FloatingManaBarComponentWidget->SetupAttachment(RootComponent);
}

void ACombatAllies::BeginPlay()
{
	Super::BeginPlay();
	//Set up properties for FloatingManaBar
	FloatingManaBarWidget = Cast<UFloatingManaBarWidget>(FloatingManaBarComponentWidget->GetWidget());
	if (IsValid(FloatingManaBarWidget)) {
		FloatingManaBarWidget->Mana = CurrentMana;
		FloatingManaBarWidget->MaxMana = MaxMana;
	}
}

void ACombatAllies::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//CombatPlayerCharacter movement control.
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ABattleManager* BattleManager = Cast<ABattleManager>(PlayerCharacter->GetBattleManager()); IsValid(BattleManager)) {
			if (IsMovingToAttackEnemy) {
				FVector DistanceVector{};
				if (IsValid(BattleManager->SelectedCombatNPC))
					DistanceVector = FVector(BattleManager->SelectedCombatNPC->GetActorLocation() - this->GetActorLocation());
				//If distance between player and selected enemy is less than 300, turn on animation
				if (DistanceVector.Length() <= 300.0) {
					IsMovingToAttackEnemy = false;
					if (UCombatCharacterAnimInstance* CombatCharacterAnimInstance = Cast<UCombatCharacterAnimInstance>(GetMesh()->GetAnimInstance()); IsValid(CombatCharacterAnimInstance))
						CombatCharacterAnimInstance->ToggleCombatCharacterIsAttacking(true);
				}
			}
			//Move player back to his start position
			else if (IsMovingToStartPosition) {
				FVector DistanceVector{};
				if (IsValid(StartLocation))
					DistanceVector = FVector(StartLocation->GetActorLocation() - this->GetActorLocation());
				//If distance between player and start position is less than 50, stop
				if (DistanceVector.Length() <= 50.0) {
					PlayerCharacter->GetBattleMenuWidget()->GetCenterMark()->SetVisibility(ESlateVisibility::Hidden);
					IsMovingToStartPosition = false;
					this->SetActorRotation(FRotator(0, 180, 0));
					BattleManager->PlayerTurnController();
				}
			}
		}
}

void ACombatAllies::StartMovingToEnemy()
{
	IsMovingToAttackEnemy = true;
	ACombatAlliesAIController* CombatAlliesAIController = Cast<ACombatAlliesAIController>(GetController());
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ABattleManager* BattleManager = Cast<ABattleManager>(PlayerCharacter->GetBattleManager()); IsValid(BattleManager)) 
			if (IsValid(CombatAlliesAIController) && IsValid(BattleManager)) 
				CombatAlliesAIController->MoveToActor(BattleManager->SelectedCombatNPC, 100.f);
}

void ACombatAllies::GetHitWithBuffOrDebuff_Implementation(const TArray<class AEffect*>& HitEffects)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager)) {
			if (IsValid(PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell())) {
				ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(),
					GetActorLocation(), GetActorRotation());
				FString TextForCombatFloatingInformationActor = FString();
				uint8 EvasionRandomNumber = FMath::RandRange(0, 100);
				int ChanceOfEvasion = SkillsSpellsAndEffectsActions::GetBuffOrDebuffEvasionChanceAfterResistances(SkillsSpellsAndEffectsActions::GetValueAfterEffects(EvasionChance + Agility * 2,
					Effects, EEffectArea::EVASION), Effects, Resistances, PlayerCharacter->GetSkillBattleMenuWidget()->GetCreatedSpell()->GetElementsAndTheirPercentagesStructs());
				if (EvasionRandomNumber <= ChanceOfEvasion) {
					TextForCombatFloatingInformationActor.Append("Miss!");
					CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
				}
				else {
					TextForCombatFloatingInformationActor.Append("Hit!");
					CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
					for (AEffect* Effect : HitEffects)
						Effects.Add(Effect);
				}
			}
		}
}

void ACombatAllies::GetHit_Implementation(int ValueOfAttack, const TArray<FElementAndItsPercentageStruct>& ContainedElements)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager)) {
			ACombatFloatingInformationActor* CombatFloatingInformationActor = GetWorld()->SpawnActor<ACombatFloatingInformationActor>(BattleManager->GetCombatFloatingInformationActorClass(), GetActorLocation(), GetActorRotation());
			FString TextForCombatFloatingInformationActor = FString();
			uint8 EvasionRandomNumber = FMath::RandRange(0, 100);
			int ChanceOfEvasion = SkillsSpellsAndEffectsActions::GetValueAfterEffects(EvasionChance + Agility * 2, Effects, EEffectArea::EVASION);
			if (EvasionRandomNumber <= ChanceOfEvasion) {
				TextForCombatFloatingInformationActor.Append("Miss!");
				CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
			}
			else {
				int ValueOfArmor = SkillsSpellsAndEffectsActions::GetValueAfterEffects(ArmorValue, Effects, EEffectArea::ARMOR);
				int ValueOfAttackWithResistances = SkillsSpellsAndEffectsActions::GetAttackOrRestorationValueAfterResistances(ValueOfAttack, Effects, Resistances, ContainedElements);
				if (CurrentHP - (ValueOfAttackWithResistances - ValueOfArmor / 10) < 0)
					CurrentHP = 0;
				else
					CurrentHP -= (ValueOfAttackWithResistances - ValueOfArmor / 10);
				if (IsValid(FloatingHealthBarWidget))
					FloatingHealthBarWidget->HP = CurrentHP;
				TextForCombatFloatingInformationActor.AppendInt(ValueOfAttackWithResistances - ValueOfArmor / 10);
				CombatFloatingInformationActor->SetCombatFloatingInformationText(FText::FromString(TextForCombatFloatingInformationActor));
			}
			UCombatCharacterAnimInstance* AnimInstance = Cast<UCombatCharacterAnimInstance>(GetMesh()->GetAnimInstance());
			if (IsValid(AnimInstance)) {
				if (CurrentHP <= 0)
					AnimInstance->ToggleCombatCharacterIsDead(true);
				else
					AnimInstance->ToggleCombatCharacterGotHit(true);
			}
		}
}

void ACombatAllies::StartMovingToStartLocation()
{
	IsMovingToStartPosition = true;
	ACombatAlliesAIController* CombatAlliesAIController = Cast<ACombatAlliesAIController>(GetController());
	if (IsValid(CombatAlliesAIController))
		CombatAlliesAIController->MoveToActor(StartLocation, 10.f);
}

UFloatingManaBarWidget* ACombatAllies::GetFloatingManaBarWidget() const
{
	return FloatingManaBarWidget;
}
