// Fill out your copyright notice in the Description page of Project Settings.


#include "NonCombatEnemyNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/KismetMathLibrary.h>

ANonCombatEnemyNPC::ANonCombatEnemyNPC()
{
	//Create widget for NonCombatEnemyDetectionBarWidget
	NonCombatEnemyDetectionBarComponentWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	NonCombatEnemyDetectionBarComponentWidget->SetWidgetSpace(EWidgetSpace::Screen);
	NonCombatEnemyDetectionBarComponentWidget->SetupAttachment(RootComponent);
}

void ANonCombatEnemyNPC::BeginPlay()
{
	Super::BeginPlay();
	ForwardMarker = Cast<UBoxComponent>(GetComponentByClass(UBoxComponent::StaticClass()));
	Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	NonCombatEnemyNPCAIController = Cast<ANonCombatEnemyNPCAIController>(this->GetController());

	//Set up properties for NonCombatEnemyDetectionBarWidget
	NonCombatEnemyDetectionBarComponentWidget->SetWidgetClass(NonCombatEnemyDetectionBarClass);
	NonCombatEnemyDetectionBarWidget = Cast<UNonCombatEnemyDetectionBarWidget>(NonCombatEnemyDetectionBarComponentWidget->GetWidget());
	if (NonCombatEnemyDetectionBarWidget) {
		NonCombatEnemyDetectionBarWidget->Detection = NonCombatEnemyNPCAIController->GetPlayerDetection();
	}
}

void ANonCombatEnemyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (NonCombatEnemyNPCAIController) {
		NonCombatEnemyNPCAIController->SetBlackboardDistanceToThePlayer((Player->GetActorLocation() - GetActorLocation()).Length());
		if (NonCombatEnemyNPCAIController->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) && (Player->GetActorLocation()-this->GetActorLocation()).Length() <= 200.f) {
			FVector FacingVector = Player->GetActorLocation() - this->GetActorLocation();
			FRotator FacingRotator = FacingVector.Rotation();
			FacingRotator.Pitch = 0.f;
			this->SetActorRotation(FacingRotator, ETeleportType::None);
		}
	}
}

TArray<TSubclassOf<ACombatEnemyNPC>> ANonCombatEnemyNPC::GetBattleEnemies()
{
	return BattleEnemies;
}

UBoxComponent* ANonCombatEnemyNPC::GetForwardMarker()
{
	return ForwardMarker;
}

UNonCombatEnemyDetectionBarWidget* ANonCombatEnemyNPC::GetNonCombatEnemyDetectionBarWidget()
{
	return NonCombatEnemyDetectionBarWidget;
}
