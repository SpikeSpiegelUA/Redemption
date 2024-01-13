// Fill out your copyright notice in the Description page of Project Settings.


#include "PartyMenu.h"

bool UPartyMenu::Initialize()
{
	const bool bSuccess = Super::Initialize();
	//if (IsValid(UseButtonWithNeighbors)) {
	//	UseButtonWithNeighbors->OnClicked.AddDynamic(this, &USkillBattleMenu::UseButtonWithNeighborsOnClicked);
	//	UseButtonWithNeighbors->OnHovered.AddDynamic(this, &USkillBattleMenu::UseButtonWithNeighborsOnHovered);
	//}
	//if (IsValid(BackButtonWithNeighbors)) {
	//	BackButtonWithNeighbors->OnClicked.AddDynamic(this, &USkillBattleMenu::BackButtonWithNeighborsOnClicked);
	//	BackButtonWithNeighbors->OnHovered.AddDynamic(this, &USkillBattleMenu::BackButtonWithNeighborsOnHovered);
	//}
	if (!bSuccess) return false;
	return bSuccess;
}

void UPartyMenu::NativeConstruct()
{
	Super::NativeConstruct();
}