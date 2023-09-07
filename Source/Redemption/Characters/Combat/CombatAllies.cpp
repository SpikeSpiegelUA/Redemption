// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAllies.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\Combat\CombatAlliesAIController.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\Combat\CombatCharacterAnimInstance.h"
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
				FVector DistanceVector;
				if (IsValid(BattleManager->SelectedCombatNPC))
					DistanceVector = FVector(BattleManager->SelectedCombatNPC->GetActorLocation() - this->GetActorLocation());
				//If distance between player and selected enemy is less than 160, turn on animation
				if (DistanceVector.Length() <= 400.0) {
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
				//If distance between player and start position is less than 100, stop
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
				CombatAlliesAIController->MoveToActor(BattleManager->SelectedCombatNPC, 10.f);
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
