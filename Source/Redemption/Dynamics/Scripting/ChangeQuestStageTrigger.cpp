// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeQuestStageTrigger.h"
#include "Components/BoxComponent.h"
#include "Redemption/GameInstance/RedemptionGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include <Redemption/Miscellaneous/RedemptionGameModeBase.h>
#include "Redemption/Dynamics/Gameplay/Managers/QuestManager.h"

AChangeQuestStageTrigger::AChangeQuestStageTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("Box Component"));
}

void AChangeQuestStageTrigger::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AChangeQuestStageTrigger::OnOverlapBegin);
	if (const auto* const GameInstance = GetWorld()->GetGameInstance<URedemptionGameInstance>(); IsValid(GameInstance))
		Execute_LoadObjectFromGameInstance(this, GameInstance);
	if (Activated)
		SetActorHiddenInGame(false);
}

void AChangeQuestStageTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase))
		if(AQuestManager* QuestManager = RedemptionGameModeBase->GetQuestManager(); IsValid(QuestManager)){
			for (FQuestAndItsStage QuestAndItsStage : QuestManager->GetActiveOrFinishedQuestsAndTheirStages()) {
				if (QuestAndItsStage.QuestClass == QuestAndItsStageToTrigger.QuestClass &&
					QuestAndItsStage.QuestStage == QuestAndItsStageToTrigger.QuestStage) {
					QuestAndItsStage.QuestStage = NewStage;
					break;
				}
			}
		}
	DeactivationAndDestroyLogic();
}
