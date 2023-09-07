// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ThrowItem.h"
#include "C:\UnrealEngineProjects\Redemption\Source\Redemption\Characters\Animation\Combat\CombatAlliesAnimInstance.h"
#include <Kismet/KismetMathLibrary.h>
#include <Redemption/Dynamics/Gameplay/Combat/ItemObject.h>
#include <Kismet/GameplayStatics.h>

void UAnimNotify_ThrowItem::SpawnItemObject(AAssaultItem* const& AssaultItem, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, ACombatNPC* const& CombatNPC)
{
	FTransform SpawnTransform = MeshComp->GetSocketTransform(FName(TEXT("RightHandIndex3")), ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParemeters;
	AItemObject* SpawnedItemObject = MeshComp->GetWorld()->SpawnActor<AItemObject>(AssaultItem->GetItemObjectClass(), SpawnTransform, ActorSpawnParemeters);
	SpawnedItemObject->SetItem(AssaultItem);
	SpawnedItemObject->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnedItemObject->GetActorLocation(), CombatNPC->Target->GetActorLocation()));

}

void UAnimNotify_ThrowItem::SpawnItemObject(ADebuffItem* const& DebuffItem, USkeletalMeshComponent* const& MeshComp, ABattleManager* const& BattleManager, ACombatNPC* const& CombatNPC)
{
	FTransform SpawnTransform = MeshComp->GetSocketTransform(FName(TEXT("RightHandIndex3")), ERelativeTransformSpace::RTS_World);
	FActorSpawnParameters ActorSpawnParemeters;
	AItemObject* SpawnedItemObject = MeshComp->GetWorld()->SpawnActor<AItemObject>(DebuffItem->GetItemObjectClass(), SpawnTransform, ActorSpawnParemeters);
	SpawnedItemObject->SetItem(DebuffItem);
	SpawnedItemObject->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(SpawnedItemObject->GetActorLocation(), CombatNPC->Target->GetActorLocation()));
}

void UAnimNotify_ThrowItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	//Spawn Spell Object and rotate it towards the selected enemy
	if(IsValid(MeshComp->GetWorld()))
	if (IsValid(MeshComp->GetWorld()) && IsValid(MeshComp->GetWorld()->GetFirstPlayerController()))
		if (ACombatNPC* CombatNPC = Cast<ACombatNPC>(MeshComp->GetOwner()); IsValid(CombatNPC))
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(MeshComp->GetWorld()->GetFirstPlayerController()->GetCharacter()))
				if (ABattleManager* BattleManager = PlayerCharacter->GetBattleManager(); IsValid(BattleManager)) {
					if (AAssaultItem* AssaultItem = Cast<AAssaultItem>(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem()); IsValid(AssaultItem)) {
						SpawnItemObject(AssaultItem, MeshComp, BattleManager, CombatNPC);
						UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->GetUseAssaultSoundCue());
					}
					else if (ADebuffItem* DebuffItem = Cast<ADebuffItem>(PlayerCharacter->GetInventoryMenuWidget()->GetPickedItem()); IsValid(DebuffItem)) {
						SpawnItemObject(DebuffItem, MeshComp, BattleManager, CombatNPC);
						UGameplayStatics::PlaySound2D(GetWorld(), PlayerCharacter->GetAudioManager()->GetUseDebuffSoundCue());
					}
				}
}