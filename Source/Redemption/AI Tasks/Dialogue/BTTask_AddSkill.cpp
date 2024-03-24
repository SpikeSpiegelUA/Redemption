// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AddSkill.h"
#include "Redemption/Characters/Player/PlayerCharacter.h"

UBTTask_AddSkill::UBTTask_AddSkill(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Add Skill";
}

EBTNodeResult::Type UBTTask_AddSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (!IsValid(PlayerCharacter))
		return EBTNodeResult::Failed;

	PlayerCharacter->AddNewSkillToAvailableSkills(SkillToAdd);

	return EBTNodeResult::Succeeded;
}
