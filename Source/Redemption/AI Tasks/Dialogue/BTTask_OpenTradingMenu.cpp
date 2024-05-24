// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_OpenTradingMenu.h"
#include "Redemption/UI/HUD/Dialogue/DialogueBox.h"
#include "..\UI\Menus\TradingMenu.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "..\Public\UIManagerWorldSubsystem.h"
#include "../Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

UBTTask_OpenTradingMenu::UBTTask_OpenTradingMenu(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
	NodeName = "Open trading menu";
}

EBTNodeResult::Type UBTTask_OpenTradingMenu::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!IsValid(MyBlackboard) || !IsValid(PlayerController) || !IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	UDialogueBox* DialogueBoxWidget = Cast<UDialogueBox>(MyBlackboard->GetValueAsObject(DialogueBoxWidgetKeySelector.SelectedKeyName));

	if (!IsValid(DialogueBoxWidget))
		return EBTNodeResult::Failed;

	UUIManagerWorldSubsystem* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	ARedemptionGameModeBase* RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(UIManagerWorldSubsystem) || !IsValid(RedemptionGameModeBase))
		return EBTNodeResult::Failed;

	DialogueBoxWidget->RemoveFromParent();
	if (UIManagerWorldSubsystem->TradingMenuWidget = CreateWidget<UTradingMenu>(PlayerController, RedemptionGameModeBase->GetTradingMenuClass()); IsValid(UIManagerWorldSubsystem->TradingMenuWidget)) {
		UIManagerWorldSubsystem->TradingMenuWidget->AddToViewport();
		UIManagerWorldSubsystem->TradingMenuWidget->InitializeTradingMenu(Cast<ATraderNPC>(OwnerComp.GetAIOwner()->GetCharacter()));
	}

	PlayerCharacter->IsTrading = true;

	return EBTNodeResult::InProgress;
}

void UBTTask_OpenTradingMenu::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!PlayerCharacter->IsTrading) {
		if(auto* const UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>(); IsValid(UIManagerWorldSubsystem))
			UIManagerWorldSubsystem->DialogueBoxWidget->AddToViewport();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
