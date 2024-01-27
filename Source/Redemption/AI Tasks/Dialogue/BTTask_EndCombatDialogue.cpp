// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EndCombatDialogue.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

UBTTask_EndCombatDialogue::UBTTask_EndCombatDialogue(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "End the combat dialogue";
}

EBTNodeResult::Type UBTTask_EndCombatDialogue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	PlayerCharacter->GetBattleMenuWidget()->GetMenuBorder()->SetVisibility(ESlateVisibility::Hidden);
	PlayerCharacter->GetBattleMenuWidget()->IsPreparingToTalk = false;
	PlayerCharacter->GetDialogueBoxWidget()->RemoveFromParent();
	PlayerCharacter->GetAlliesInfoBarsWidget()->AddToViewport();
	PlayerCharacter->IsInDialogue = false;

	PlayerCharacter->GetAudioManager()->DungeonCombatBackgroundMusicAudioComponents[PlayerCharacter->GetAudioManager()->IndexInArrayOfCurrentPlayingBGMusic]->SetPaused(false);
	PlayerCharacter->GetAudioManager()->GetDungeonTalkBackgroundMusicAudioComponent_Daat()->SetPaused(true);

	for (int8 Index = PlayerCharacter->GetResponsesBox()->GetResponseVerticalBox()->GetAllChildren().Num() - 1; Index >= 0; Index--) 
		PlayerCharacter->GetResponsesBox()->GetResponseVerticalBox()->GetChildAt(Index)->RemoveFromParent();

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("FirstDialoguePassed", false);
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>("IsInDialogue", false);

	return EBTNodeResult::Succeeded;
}
