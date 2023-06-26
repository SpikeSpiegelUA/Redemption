// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\AI Controllers\NonCombatEnemyNPCAIController.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\CharacterInTheWorld.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Enemies\NonCombatEnemyNPC.h"
#include <D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

ANonCombatEnemyNPCAIController::ANonCombatEnemyNPCAIController(const FObjectInitializer& ObjectInitializer)
{

}

void ANonCombatEnemyNPCAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter()); IsValid(PlayerCharacter)) {
		if (ANonCombatEnemyNPC* ThisEnemy = Cast<ANonCombatEnemyNPC>(GetPawn()); IsValid(ThisEnemy)) {
			//Get angle between player and enemy's forward vector
			FVector FromPlayerToEnemy = GetPawn()->GetActorLocation() - PlayerCharacter->GetActorLocation();
			FVector WorldForwardVector = ThisEnemy->GetForwardMarker()->GetComponentLocation() - ThisEnemy->GetActorLocation();
			float AimAtAngle = ((acosf(FVector::DotProduct(FromPlayerToEnemy.GetSafeNormal(), WorldForwardVector.GetSafeNormal()))) * (180 / 3.1415926));
			//Detection system
			if (AimAtAngle >= 100.f && (PlayerCharacter->GetActorLocation() - GetPawn()->GetActorLocation()).Length() <= 1600.f) {
				FHitResult HitResult(ForceInit);
				FCollisionQueryParams* CQP = new FCollisionQueryParams();
				CQP->AddIgnoredActor(GetPawn());
				GetWorld()->LineTraceSingleByChannel(HitResult, GetPawn()->GetActorLocation(), PlayerCharacter->GetActorLocation(), ECC_Visibility, *CQP);
				if (IsValid(HitResult.GetActor()) && HitResult.GetActor()->ActorHasTag(FName("Player"))) {
					GetWorld()->GetTimerManager().ClearTimer(DeductPlayerDetectionHandle);
					if (!GetWorld()->GetTimerManager().IsTimerActive(AddPlayerDetectionHandle))
						GetWorld()->GetTimerManager().SetTimer(AddPlayerDetectionHandle, this, &ANonCombatEnemyNPCAIController::AddPlayerDetection, 1.f, false);
				}
				else if (IsValid(HitResult.GetActor()) && !HitResult.GetActor()->ActorHasTag(FName("Player"))) {
					GetWorld()->GetTimerManager().ClearTimer(AddPlayerDetectionHandle);
					if (!GetWorld()->GetTimerManager().IsTimerActive(DeductPlayerDetectionHandle))
						GetWorld()->GetTimerManager().SetTimer(DeductPlayerDetectionHandle, this, &ANonCombatEnemyNPCAIController::DeductPlayerDetection, 0.5f, false);
				}
			}
			else if(AimAtAngle < 100.f || (PlayerCharacter->GetActorLocation() - GetPawn()->GetActorLocation()).Length() > 1600.f) {
					GetWorld()->GetTimerManager().ClearTimer(AddPlayerDetectionHandle);
					if (!GetWorld()->GetTimerManager().IsTimerActive(DeductPlayerDetectionHandle))
						GetWorld()->GetTimerManager().SetTimer(DeductPlayerDetectionHandle, this, &ANonCombatEnemyNPCAIController::DeductPlayerDetection, 0.5f, false);
			}
			if ((PlayerCharacter->GetActorLocation() - GetPawn()->GetActorLocation()).Length() <= 500.f) {
				PlayerDetection = 4;
				GetWorld()->GetTimerManager().ClearTimer(DeductPlayerDetectionHandle);
				GetWorld()->GetTimerManager().ClearTimer(AddPlayerDetectionHandle);
				AddPlayerDetection();
			}
		}
	}
}
void ANonCombatEnemyNPCAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{

}

void ANonCombatEnemyNPCAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ACharacterInTheWorld* Chr = Cast<ACharacterInTheWorld>(InPawn);
	if (IsValid(Chr) && IsValid(Chr->GetTreeAsset())) {

			//Initialize the Blackboard
			BlackboardComponent->InitializeBlackboard(*Chr->GetTreeAsset()->BlackboardAsset);
			//Set Blackboard Key IDs
			CanSeePlayerKeyID = BlackboardComponent->GetKeyID("CanSeePlayer");
			//Start the Behavior Tree
			BehaviorTreeComponent->StartTree(*Chr->GetTreeAsset());
	}
}

void ANonCombatEnemyNPCAIController::AddPlayerDetection()
{
	if ((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetPawn()->GetActorLocation()).Length() <= 500)
		PlayerDetection += 4;
	else
		PlayerDetection += 1;
	if (PlayerDetection >= 4) {
		PlayerDetection = 4;
		BlackboardComponent->SetValue<UBlackboardKeyType_Bool>("CanSeePlayer", true);
		GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 260.f;
	}
	if(PlayerDetection < 4)
	GetWorld()->GetTimerManager().SetTimer(AddPlayerDetectionHandle, this, &ANonCombatEnemyNPCAIController::AddPlayerDetection, 1.f, false);
	ANonCombatEnemyNPC* ThisEnemy = Cast<ANonCombatEnemyNPC>(GetPawn());
	if(IsValid(ThisEnemy))
		ThisEnemy->GetNonCombatEnemyDetectionBarWidget()->Detection = PlayerDetection;
}

void ANonCombatEnemyNPCAIController::DeductPlayerDetection()
{
	PlayerDetection -= 1;
	if (PlayerDetection > 0)
		GetWorld()->GetTimerManager().SetTimer(DeductPlayerDetectionHandle, this, &ANonCombatEnemyNPCAIController::DeductPlayerDetection, 0.5f, false);
	else if (PlayerDetection <= 0) {
		PlayerDetection = 0;
		BlackboardComponent->SetValue<UBlackboardKeyType_Bool>("CanSeePlayer", false);
		GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 150.f;
	}
	ANonCombatEnemyNPC* ThisEnemy = Cast<ANonCombatEnemyNPC>(GetPawn());
	if(IsValid(ThisEnemy))
		ThisEnemy->GetNonCombatEnemyDetectionBarWidget()->Detection = PlayerDetection;
}

void ANonCombatEnemyNPCAIController::SetBlackboardDistanceToThePlayer(float Value)
{
	BlackboardComponent->SetValue<UBlackboardKeyType_Float>("DistanceToThePlayer", Value);
}

float ANonCombatEnemyNPCAIController::GetPlayerDetection() const
{
	return PlayerDetection;
}

uint8 ANonCombatEnemyNPCAIController::GetCanSeePlayerKeyID() const
{
	return CanSeePlayerKeyID;
}



