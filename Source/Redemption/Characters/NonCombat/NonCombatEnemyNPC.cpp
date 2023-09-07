// Fill out your copyright notice in the Description page of Project Settings.


#include "NonCombatEnemyNPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/KismetMathLibrary.h>

ANonCombatEnemyNPC::ANonCombatEnemyNPC()
{
	//Create widget for NonCombatEnemyDetectionBarWidget
	NonCombatEnemyDetectionBarComponentWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	NonCombatEnemyDetectionBarComponentWidget->SetWidgetSpace(EWidgetSpace::World);
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
	if (IsValid(NonCombatEnemyDetectionBarWidget)) {
		NonCombatEnemyDetectionBarWidget->Detection = NonCombatEnemyNPCAIController->GetPlayerDetection();
	}
}

void ANonCombatEnemyNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsValid(NonCombatEnemyNPCAIController)) {
		NonCombatEnemyNPCAIController->SetBlackboardDistanceToThePlayer((Player->GetActorLocation() - GetActorLocation()).Length());
		if (NonCombatEnemyNPCAIController->GetBlackboardComponent()->GetValueAsBool(FName("CanSeePlayer")) && (Player->GetActorLocation()-this->GetActorLocation()).Length() <= 200.f) {
			FVector FacingVector = Player->GetActorLocation() - this->GetActorLocation();
			FRotator FacingRotator = FacingVector.Rotation();
			FacingRotator.Pitch = 0.f;
			this->SetActorRotation(FacingRotator, ETeleportType::None);
		}
	}
	if (IsValid(NonCombatEnemyDetectionBarComponentWidget)) {
		FRotator ComponentRotation = NonCombatEnemyDetectionBarComponentWidget->GetComponentRotation();
		NonCombatEnemyDetectionBarComponentWidget->SetWorldRotation(FRotator(ComponentRotation.Pitch, 90, 0));
	}
}

TArray<TSubclassOf<ACombatEnemyNPC>> ANonCombatEnemyNPC::GetBattleEnemies() const
{
	return BattleEnemies;
}

UBoxComponent* ANonCombatEnemyNPC::GetForwardMarker() const
{
	return ForwardMarker;
}

UNonCombatEnemyDetectionBarWidget* ANonCombatEnemyNPC::GetNonCombatEnemyDetectionBarWidget() const
{
	return NonCombatEnemyDetectionBarWidget;
}
