// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AskItem.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\HUD\Dialogue\ResponseEntry.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Redemption/Miscellaneous/InventoryActions.h"

UBTTask_AskItem::UBTTask_AskItem(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
	NodeName = "Ask for an item";
}

EBTNodeResult::Type UBTTask_AskItem::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
		FString StringToSet = NPCAgreeText.ToString();
		if (CombatEnemyNPC->GetPossibleAskItems().Num() > 0) {
			uint8 RandomItem = FMath::RandRange(0, CombatEnemyNPC->GetPossibleAskItems().Num() - 1);
			StringToSet.Append("(The item you got: ");
			StringToSet.Append(CombatEnemyNPC->GetPossibleAskItems()[RandomItem]->GetDefaultObject<AGameItem>()->GetItemName().ToString());
			StringToSet.Append(")");
			AGameItem* GameItem = NewObject<AGameItem>(this, CombatEnemyNPC->GetPossibleAskItems()[RandomItem]);
			PlayerCharacter->GetGameInstance()->InstanceItemsInTheInventory.Add(CombatEnemyNPC->GetPossibleAskItems()[RandomItem]);
			if (IsValid(GameItem)) {
				//Get ScrollBox corresponding to the item's type
				UScrollBox* CurrentScrollBox = InventoryActions::FindCorrespondingScrollBox(PlayerCharacter->GetInventoryMenuWidget(), GameItem);
				//Check if this item is already in the inventory. If yes, than just add to AmountOfItems and change text, if not, then add new inventory widget
				InventoryActions::IfItemAlreadyIsInInventory(GetWorld(), CurrentScrollBox, GameItem);
				CombatEnemyNPC->AskMoneyItemSuccessChance -= 10;
			}
		}
		PlayerCharacter->GetDialogueBoxWidget()->SetDialogueText(FText::FromString(StringToSet));
	}
	else {
		PlayerCharacter->GetDialogueBoxWidget()->SetDialogueText(NPCRefuseText);
		BlackboardComponent->SetValueAsBool("PassedDialogueCheck", false);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_AskItem::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//If "Action" button was pressed, continue dialogue
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PlayerController) && PlayerController->WasInputKeyJustPressed(EKeys::E))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}