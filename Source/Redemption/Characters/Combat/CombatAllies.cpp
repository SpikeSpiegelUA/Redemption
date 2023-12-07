// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAllies.h"
#include "..\Characters\AI Controllers\Combat\CombatAlliesAIController.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\Characters\Animation\Combat\CombatAlliesAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "..\Miscellaneous\SkillsSpellsAndEffectsActions.h"
#include "..\Miscellaneous\ElementsActions.h"
#include "..\UI\HUD\FloatingManaBarWidget.h"

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
}

void ACombatAllies::StartMovingToEnemy()
{
	IsMovingToAttackEnemy = true;
	ACombatAlliesAIController* CombatAlliesAIController = Cast<ACombatAlliesAIController>(GetController());
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter))
		if (ABattleManager* BattleManager = Cast<ABattleManager>(PlayerCharacter->GetBattleManager()); IsValid(BattleManager)) 
			if (IsValid(CombatAlliesAIController) && IsValid(BattleManager)) 
				CombatAlliesAIController->MoveToActor(BattleManager->SelectedCombatNPC, 185.f);
}

void ACombatAllies::StartMovingToStartLocation()
{
	IsMovingToStartPosition = true;
	ACombatAlliesAIController* CombatAlliesAIController = Cast<ACombatAlliesAIController>(GetController());
	if (IsValid(CombatAlliesAIController))
		CombatAlliesAIController->MoveToActor(StartLocation);
}

UFloatingManaBarWidget* ACombatAllies::GetFloatingManaBarWidget() const
{
	return FloatingManaBarWidget;
}
