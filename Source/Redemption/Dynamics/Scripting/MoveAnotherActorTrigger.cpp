// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveAnotherActorTrigger.h"
#include "..\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "..\Source\Redemption\Dynamics\Gameplay\Managers\QuestManager.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Redemption/Characters/AI Controllers/NonCombat/TownNPCAIController.h"

// Sets default values
AMoveAnotherActorTrigger::AMoveAnotherActorTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Box Component"));
}

// Called when the game starts or when spawned
void AMoveAnotherActorTrigger::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMoveAnotherActorTrigger::OnOverlapBegin);
	if(const auto* const GameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(GameInstance))
		Execute_LoadObjectFromGameInstance(this, GameInstance);
	if (Activated)
		SetActorHiddenInGame(false);
}

// Called every frame
void AMoveAnotherActorTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMoveAnotherActorTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if(auto* const QuestManager = RedemptionGameModeBase->GetQuestManager(); IsValid(QuestManager))
			if (const auto* const PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter(); IsValid(PlayerCharacter))
				if (OtherActor == PlayerCharacter && IsValid(ActorToMove)) {
					for(const FQuestAndItsStage QuestAndItsStage : QuestManager->GetActiveOrFinishedQuestsAndTheirStages())
						for (FQuestAndItsStage TriggerQuestAndItsStage : QuestsAndTheirStagesToTrigger) {
							if (QuestAndItsStage.QuestClass == TriggerQuestAndItsStage.QuestClass && QuestAndItsStage.QuestStage >= TriggerQuestAndItsStage.QuestStage) {
								ActorToMove->SetActorLocation(NewLocation);
								ActorToMove->SetActorRotation(NewRotation);
								if (auto* const CharacterInTheWorld = Cast<ACharacterInTheWorld>(ActorToMove); IsValid(CharacterInTheWorld)) {
									CharacterInTheWorld->SetSmartObject(NewSmartObject);
									if (NewSmartObject == nullptr)
										if (auto* const Character = Cast<ACharacter>(CharacterInTheWorld); IsValid(Character))
											if (auto* const TownNPCAIController = Character->GetController<ATownNPCAIController>(); IsValid(TownNPCAIController))
												TownNPCAIController->DisableMainBehavior();
								}
								if (!Triggered) {
									QuestManager->AdvanceQuest(TriggerQuestAndItsStage.QuestClass);
									Triggered = true;
								}
								DeactivationAndDestroyLogic();
							}
						}
				}
}

