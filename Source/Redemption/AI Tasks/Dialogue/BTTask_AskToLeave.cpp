// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AskToLeave.h"
#include "..\Characters\Player\PlayerCharacter.h"
#include "..\UI\HUD\Dialogue\ResponseEntry.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_String.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Redemption/Miscellaneous/RedemptionGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UBTTask_AskToLeave::UBTTask_AskToLeave(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
	NodeName = "Ask to Leave";
}

EBTNodeResult::Type UBTTask_AskToLeave::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	ACombatEnemyNPC* CombatEnemyNPC = Cast<ACombatEnemyNPC>(OwnerComp.GetAIOwner()->GetCharacter());
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	UUIManagerWorldSubsystem* UIManagerWorldSubsystem = GetWorld()->GetSubsystem<UUIManagerWorldSubsystem>();
	if (!IsValid(CombatEnemyNPC) || !IsValid(BlackboardComponent) || !IsValid(PlayerCharacter) || !IsValid(UIManagerWorldSubsystem))
		return EBTNodeResult::Failed;

	uint8 RandomNumber = FMath::RandRange(0, 100);
	uint16 ChanceOfSuccess = CombatEnemyNPC->AskToLeaveSuccessChance + (PlayerCharacter->GetStat(ECharacterStats::LUCK) - CombatEnemyNPC->GetStat(ECharacterStats::LUCK)) * 2 +
		(CombatEnemyNPC->MaxHP - CombatEnemyNPC->CurrentHP) / CombatEnemyNPC->MaxHP * 10 + PlayerCharacter->GetStat(ECharacterStats::CHARISMA) / 2 +
		PlayerCharacter->GetSkill(ECharacterSkills::PERSUASION) / 10;

	UIManagerWorldSubsystem->DialogueBoxWidget->GetContinueButton()->SetVisibility(ESlateVisibility::Visible);

	if (RandomNumber < ChanceOfSuccess) {
		BlackboardComponent->SetValueAsBool("PassedDialogueCheck", true);
		UIManagerWorldSubsystem->DialogueBoxWidget->SetDialogueText(NPCAgreeText);
		if (const auto* const RedemptionGameModeBase = Cast<ARedemptionGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); IsValid(RedemptionGameModeBase)) {
			RedemptionGameModeBase->GetBattleManager()->CombatPlayerCharacter->AddSkillsProgress(ECharacterSkills::PERSUASION, 100);
			RedemptionGameModeBase->GetBattleManager()->CombatPlayerCharacter->SetSkillsLeveledUp(ESkillsLeveledUp::SkillsLeveledUpPersuasion, true);
		}
	}
	else {
		UIManagerWorldSubsystem->DialogueBoxWidget->SetDialogueText(NPCRefuseText);
		BlackboardComponent->SetValueAsBool("PassedDialogueCheck", false);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_AskToLeave::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//If "Action" button was pressed, continue dialogue
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PlayerController) && PlayerController->WasInputKeyJustPressed(EKeys::E))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
