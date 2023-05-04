// Fill out your copyright notice in the Description page of Project Settings.


#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Dynamics\Logic\Notify\PlayerMeleeAttackEndAnimNotify.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Player\PlayerCharacter.h"
#include "D:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\PlayerCharacterAnimInstance.h"
#include "EngineUtils.h"
#include "Engine.h"

void UPlayerMeleeAttackEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Hit a selected enemy
	TSubclassOf<APlayerCharacter> ClassToFind;
	ClassToFind = APlayerCharacter::StaticClass();
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(MeshComp->GetAttachmentRootActor()->GetWorld(), ClassToFind, FoundPlayers);
	if (FoundPlayers.Num() > 0) {
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(FoundPlayers[0]);
		if (PlayerCharacter)
			PlayerCharacter->Battle_IsMovingToStartPosition = true;
		UPlayerCharacterAnimInstance* AnimInstance = Cast<UPlayerCharacterAnimInstance>(MeshComp->GetAnimInstance());
		if(AnimInstance)
		    AnimInstance->SetIsAttacking(false);
	}
}