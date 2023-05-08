// Fill out your copyright notice in the Description page of Project Settings.


#include "NonCombatPunchThrowAnimNotify.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Gameplay\Managers\GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine.h"

void UNonCombatPunchThrowAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	TSubclassOf<AGameManager> ClassToFind;
	ClassToFind = AGameManager::StaticClass();
	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(MeshComp->GetAttachmentRootActor()->GetWorld(), ClassToFind, FoundManagers);
	float DistanceBetweenEnemyAndPlayer = (MeshComp->GetAttachmentRootActor()->GetWorld()->GetFirstPlayerController()->GetCharacter()->GetActorLocation() - MeshComp->GetAttachmentRootActor()->GetActorLocation()).Length();
	if (FoundManagers.Num() > 0 && DistanceBetweenEnemyAndPlayer <= 250.f) {
		AGameManager* GameManager = Cast<AGameManager>(FoundManagers[0]);
		GameManager->StartBattle(MeshComp->GetAttachmentRootActor());
	}
}
