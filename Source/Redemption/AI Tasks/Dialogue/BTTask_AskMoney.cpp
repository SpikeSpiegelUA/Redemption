// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AskMoney.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\HUD\Dialogue\ResponseEntry.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_AskMoney::UBTTask_AskMoney(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
	NodeName = "Ask for money";
}

EBTNodeResult::Type UBTTask_AskMoney::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(OwnerComp.GetAIOwner()->GetCharacter());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(CombatEnemyNPC) || !IsValid(BlackboardComponent) || !IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	uint8 RandomNumber = FMath::RandRange(0, 100);
	uint16 ChanceOfSuccess = CombatEnemyNPC->AskMoneyItemSuccessChance + (PlayerCharacter->Luck - CombatEnemyNPC->GetLuck()) * 2 +
		(CombatEnemyNPC->MaxHP - CombatEnemyNPC->CurrentHP) / CombatEnemyNPC->MaxHP * 10;

	PlayerCharacter->GetDialogueBoxWidget()->GetContinueButton()->SetVisibility(ESlateVisibility::Visible);

	if (RandomNumber < ChanceOfSuccess) {
		BlackboardComponent->SetValueAsBool("PassedDialogueCheck", true);
		uint16 RandomSumOfMoney = FMath::RandRange(1, 10 * PlayerCharacter->Level + PlayerCharacter->Luck * 10);
		FString StringToSet = NPCAgreeText.ToString();
		StringToSet.Append("(The amount of gold you got: ");
		StringToSet.AppendInt(RandomSumOfMoney);
		StringToSet.Append(")");
		PlayerCharacter->GetDialogueBoxWidget()->SetDialogueText(FText::FromString(StringToSet));
		PlayerCharacter->Gold += RandomSumOfMoney;
		CombatEnemyNPC->AskMoneyItemSuccessChance -= 10;
	}
	else {
		PlayerCharacter->GetDialogueBoxWidget()->SetDialogueText(NPCRefuseText);
		BlackboardComponent->SetValueAsBool("PassedDialogueCheck", false);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_AskMoney::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//If "Action" button was pressed, continue dialogue
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PlayerController) && PlayerController->WasInputKeyJustPressed(EKeys::E))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

